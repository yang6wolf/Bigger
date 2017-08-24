// Tencent is pleased to support the open source community by making Mars available.
// Copyright (C) 2016 THL A29 Limited, a Tencent company. All rights reserved.

// Licensed under the MIT License (the "License"); you may not use this file except in 
// compliance with the License. You may obtain a copy of the License at
// http://opensource.org/licenses/MIT

// Unless required by applicable law or agreed to in writing, software distributed under the License is
// distributed on an "AS IS" basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
// either express or implied. See the License for the specific language governing permissions and
// limitations under the License.


/*
 * appender.h
 *
 *  Created on: 2013-3-7
 *      Author: yerungui
 */

#ifdef _WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "log/appender.h"
#include <stdio.h>

#ifdef _WIN32
#define PRIdMAX "lld"
#define snprintf _snprintf
#define strcasecmp _stricmp
#define S_ISDIR(m)  (((m) & S_IFMT) == S_IFDIR)
#define fileno _fileno
#else
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <sys/mount.h>
#endif

#include <ctype.h>
#include <assert.h>

#include <unistd.h>
#include <zlib.h>

#include <string>
#include <algorithm>
#include <codecvt>

#include "boost/bind.hpp"
#include "boost/iostreams/device/mapped_file.hpp"
#include "boost/filesystem.hpp"

#include "comm/thread/lock.h"
#include "comm/thread/condition.h"
#include "comm/thread/thread.h"
#include "comm/scope_recursion_limit.h"
#include "comm/bootrun.h"
#include "comm/tickcount.h"
#include "comm/autobuffer.h"
#include "comm/ptrbuffer.h"
#include "comm/xlogger/xloggerbase.h"
#include "comm/time_utils.h"
#include "comm/strutil.h"
#include "comm/mmap_util.h"
#include "comm/tickcount.h"
#include "comm/verinfo.h"

#include "log_buffer.h"

#define LOG_EXT "plog"

extern void log_formater(const XLoggerInfo* _info, const char* _logbody, PtrBuffer& _log);
extern void ConsoleLog(const XLoggerInfo* _info, const char* _log);
extern void __bigger_ConsoleLog(const char* _log);

static TAppenderMode sg_mode = kAppednerAsync;

static std::string sg_logdir;
static std::string sg_cache_logdir;
static std::string sg_logfileprefix;

static Mutex sg_mutex_log_file;
static FILE* sg_logfile = NULL;
static time_t sg_openfiletime = 0;
static std::string sg_current_dir;

static Mutex sg_mutex_buffer_async;
#ifdef _WIN32
static Condition& sg_cond_buffer_async = *(new Condition());  // 改成引用, 避免在全局释放时执行析构导致crash
#else
static Condition sg_cond_buffer_async;
#endif

static LogBuffer* sg_log_buff = NULL;

static volatile bool sg_log_close = true;

static Tss sg_tss_dumpfile(&free);

#ifdef DEBUG
static bool sg_consolelog_open = true;
#else
static bool sg_consolelog_open = false;
#endif

static uint64_t sg_max_file_size = 0; // 0, will not split log file.

static void __async_log_thread();
static Thread sg_thread_async(&__async_log_thread);

static const unsigned int kBufferBlockLength = 150 * 1024;
static const long kMaxLogAliveTime = 10 * 24 * 60 * 60;	// 10 days in second

static std::string sg_log_extra_msg;

static boost::iostreams::mapped_file sg_mmmap_file;

#define UNDEFINED_SIZE 2000000000
#define MAX_SIZE 5242880
#define MIN_SIZE 3145728

static bool __is_crypt;
static uintmax_t __file_size = UNDEFINED_SIZE;
static const char kMagicAsyncStart ='\x07';
static char* __logdata_temp = NULL;

namespace {
class ScopeErrno {
  public:
    ScopeErrno() {m_errno = errno;}
    ~ScopeErrno() {errno = m_errno;}

  private:
    ScopeErrno(const ScopeErrno&);
    const ScopeErrno& operator=(const ScopeErrno&);

  private:
    int m_errno;
};

#define SCOPE_ERRNO() SCOPE_ERRNO_I(__LINE__)
#define SCOPE_ERRNO_I(line) SCOPE_ERRNO_II(line)
#define SCOPE_ERRNO_II(line) ScopeErrno __scope_errno_##line

}

static void __make_logfilename(const std::string& _logdir, const char* _prefix, const std::string& _fileext, char* _filepath, unsigned int _len) {
    std::string logfilepath = _logdir;
    logfilepath += "/";
    logfilepath += _prefix;
    logfilepath += ".";
    logfilepath += _fileext;
    strncpy(_filepath, logfilepath.c_str(), _len - 1);
    _filepath[_len - 1] = '\0';
}

static void __del_files(const std::string& _forder_path) {
    
    boost::filesystem::path path(_forder_path);
    if (!boost::filesystem::is_directory(path)) {
        return;
    }
    
    boost::filesystem::directory_iterator end_iter;
    for (boost::filesystem::directory_iterator iter(path); iter != end_iter; ++iter) {
        if (boost::filesystem::is_regular_file(iter->status()))
        {
            boost::filesystem::remove(iter->path());
        }
    }
}

static void __del_timeout_file(const std::string& _log_path) {
    time_t now_time = time(NULL);
    
    boost::filesystem::path path(_log_path);
    
    if (boost::filesystem::exists(path) && boost::filesystem::is_directory(path)){
        boost::filesystem::directory_iterator end_iter;
        for (boost::filesystem::directory_iterator iter(path); iter != end_iter; ++iter) {
            time_t fileModifyTime = boost::filesystem::last_write_time(iter->path());
            
            if (now_time > fileModifyTime && now_time - fileModifyTime > kMaxLogAliveTime) {
                if (boost::filesystem::is_regular_file(iter->status())) {
                    boost::filesystem::remove(iter->path());
                }
                else if (boost::filesystem::is_directory(iter->status())) {
                    __del_files(iter->path().string());
                }
            }
        }
    }
}

static bool __append_file(const std::string& _src_file, const std::string& _dst_file) {
    if (_src_file == _dst_file) {
        return false;
    }

    if (!boost::filesystem::exists(_src_file)){
        return false;
    }
    
    if (0 == boost::filesystem::file_size(_src_file)){
        return true;
    }

    FILE* src_file = fopen(_src_file.c_str(), "rb");

    if (NULL == src_file) {
        return false;
    }

    FILE* dest_file = fopen(_dst_file.c_str(), "ab");

    if (NULL == dest_file) {
        fclose(src_file);
        return false;
    }

    fseek(src_file, 0, SEEK_END);
    long src_file_len = ftell(src_file);
    long dst_file_len = ftell(dest_file);
    fseek(src_file, 0, SEEK_SET);

    char buffer[4096] = {0};

    while (true) {
        if (feof(src_file)) break;

        size_t read_ret = fread(buffer, 1, sizeof(buffer), src_file);

        if (read_ret == 0)   break;

        if (ferror(src_file)) break;

        fwrite(buffer, 1, read_ret, dest_file);

        if (ferror(dest_file))  break;
    }

    if (dst_file_len + src_file_len > ftell(dest_file)) {
        ftruncate(fileno(dest_file), dst_file_len);
        fclose(src_file);
        fclose(dest_file);
        return false;
    }

    fclose(src_file);
    fclose(dest_file);

    return true;
}

static void __move_old_files(const std::string& _src_path, const std::string& _dest_path, const std::string& _nameprefix) {
    if (_src_path == _dest_path) {
        return;
    }

    boost::filesystem::path path(_src_path);
    if (!boost::filesystem::is_directory(path)) {
        return;
    }
    
    ScopedLock lock_file(sg_mutex_log_file);
    
    struct timeval tv;
    gettimeofday(&tv, NULL);
    char logfilepath[1024] = {0};
    
    boost::filesystem::directory_iterator end_iter;
    for (boost::filesystem::directory_iterator iter(path); iter != end_iter; ++iter) {
        
        if (!strutil::StartsWith(iter->path().filename().string(), _nameprefix) || !strutil::EndsWith(iter->path().string(), LOG_EXT)) {
            continue;
        }
        
        __make_logfilename(_dest_path, sg_logfileprefix.c_str(), LOG_EXT, logfilepath , 1024);
        
        if (!__append_file(iter->path().string(), logfilepath)) {
            break;
        }
        
        boost::filesystem::remove(iter->path());
        memset(logfilepath, 0, sizeof(logfilepath));
    }
}

static void __writetips2console(const char* _tips_format, ...) {
    
    if (NULL == _tips_format) {
        return;
    }
    
    XLoggerInfo info;
    memset(&info, 0, sizeof(XLoggerInfo));
    
    char tips_info[4096] = {0};
    va_list ap;
    va_start(ap, _tips_format);
    vsnprintf(tips_info, sizeof(tips_info), _tips_format, ap);
    va_end(ap);
    ConsoleLog(&info, tips_info);
}

static void __cutfile(const std::string& _log_dir) {
    char logfilepath[1024] = {0};
    __make_logfilename(_log_dir, sg_logfileprefix.c_str(), LOG_EXT, logfilepath, 1024);
    
    if (sg_logfile != NULL) {
        fclose(sg_logfile);
        sg_logfile = NULL;
    }
    
    FILE *file_temp = fopen(logfilepath, "rb");
    uintmax_t cut_count = __file_size - MIN_SIZE;
    fseek(file_temp, cut_count, SEEK_SET);
    
    if (__logdata_temp != NULL) {
        delete[] __logdata_temp;
        __logdata_temp = NULL;
    }
    
    __logdata_temp = new char[MIN_SIZE];
    
    fread(__logdata_temp, 1, MIN_SIZE, file_temp);
    fclose(file_temp);
    
    if (!mars_boost::filesystem::remove(logfilepath)) {
        printf("remove file error when cut file!!!");
        delete[] __logdata_temp;
        return;
    }
    
    sg_logfile = fopen(logfilepath, "ab");
    __file_size = 0;
    
    char u8_byte_str[3] = {(char)0xEF, (char)0xBB, (char)0xBF};
    if (!fwrite(u8_byte_str, 1, sizeof(u8_byte_str), sg_logfile))
        printf("write BOM error when cut file!!!");
    else
        __file_size += sizeof(u8_byte_str);
    
    int _pos = 0;
    if (__is_crypt) {
        for (_pos = 0; _pos < MIN_SIZE; _pos++) {
            if (__logdata_temp[_pos] == kMagicAsyncStart)
                break;
        }
    }
    
    fwrite(__logdata_temp + _pos, 1, MIN_SIZE - _pos, sg_logfile);
    fclose(sg_logfile);
    
    __file_size += MIN_SIZE - _pos;
    sg_logfile = NULL;
    delete[] __logdata_temp;
    __logdata_temp = NULL;
}

static bool __writefile(const void* _data, size_t _len, FILE* _file) {
    if (NULL == _file) {
        assert(false);
        return false;
    }

    long before_len = ftell(_file);
    if (before_len < 0) return false;
    
    if (1 != fwrite(_data, _len, 1, _file)) {
        int err = ferror(_file);

        __writetips2console("write file error:%d", err);


        ftruncate(fileno(_file), before_len);
        fseek(_file, 0, SEEK_END);

        char err_log[256] = {0};
        snprintf(err_log, sizeof(err_log), "\nwrite file error:%d\n", err);

        AutoBuffer tmp_buff;
        sg_log_buff->Write(err_log, strnlen(err_log, sizeof(err_log)), tmp_buff);
        

        fwrite(tmp_buff.Ptr(), tmp_buff.Length(), 1, _file);

        return false;
    }
    __file_size += _len;
    return true;
}

static bool __openlogfile(const std::string& _log_dir) {
    if (sg_logdir.empty()) return false;

    struct timeval tv;
    gettimeofday(&tv, NULL);

    if (NULL != sg_logfile) {
        time_t sec = tv.tv_sec;
        tm tcur = *localtime((const time_t*)&sec);
        tm filetm = *localtime(&sg_openfiletime);

        if (filetm.tm_year == tcur.tm_year && filetm.tm_mon == tcur.tm_mon && filetm.tm_mday == tcur.tm_mday && sg_current_dir == _log_dir) return true;

        fclose(sg_logfile);
        sg_logfile = NULL;
    }

    static time_t s_last_time = 0;
    static uint64_t s_last_tick = 0;
    static char s_last_file_path[1024] = {0};

    uint64_t now_tick = gettickcount();
    time_t now_time = tv.tv_sec;

    sg_openfiletime = tv.tv_sec;
    sg_current_dir = _log_dir;

    char logfilepath[1024] = {0};
    __make_logfilename(_log_dir, sg_logfileprefix.c_str(), LOG_EXT, logfilepath, 1024);

    if (now_time < s_last_time) {
        sg_logfile = fopen(s_last_file_path, "ab");

		if (NULL == sg_logfile) {
            __writetips2console("open file error:%d %s, path:%s", errno, strerror(errno), s_last_file_path);
        }

#ifdef __APPLE__
        assert(sg_logfile);
#endif
        return NULL != sg_logfile;
    }
    
    bool file_exist = false;  //表示文件打开之前是否存在。当日志文件存在时为true，为false时新建的日志文件要加上BOM头。
    if (__file_size != UNDEFINED_SIZE || mars_boost::filesystem::exists(logfilepath)) {
        file_exist = true;
    }
    
    sg_logfile = fopen(logfilepath, "ab");
    
    if (sg_logfile != NULL && file_exist && __file_size == UNDEFINED_SIZE) {
        char str_temp[4] = {0, 0, 0, 0};
        FILE *file_temp = fopen(logfilepath, "rb");
        
        if (file_temp != NULL) {
            if (!fread(str_temp, 1, sizeof(str_temp), file_temp)) {
                printf("read file error!!!");
            }
            fclose(file_temp);
            file_temp = NULL;
        }
        
        if ((str_temp[3] == kMagicAsyncStart && !__is_crypt) || (str_temp[3] != kMagicAsyncStart && __is_crypt)) {//判断加密/不加密是否切换，如果切换了则删除之前的文件。
            fclose(sg_logfile);
            sg_logfile = NULL;
            if (!mars_boost::filesystem::remove(logfilepath)) {
                printf("remove file error!!!");
                __file_size = mars_boost::filesystem::file_size(logfilepath);
            }
            else {
                file_exist = false;
                __file_size = 0;
            }
            sg_logfile = fopen(logfilepath, "ab");
        }
        else
            __file_size = mars_boost::filesystem::file_size(logfilepath);
    }
    
    if (sg_logfile != NULL && !file_exist) {//在文件开头写入BOM头
        __file_size = 0;
        char u8_byte_str[3] = {(char)0xEF, (char)0xBB, (char)0xBF};
        if (!fwrite(u8_byte_str, 1, sizeof(u8_byte_str), sg_logfile))
            printf("write BOM error!!!");
        else
            __file_size += sizeof(u8_byte_str);
    }
    
	if (NULL == sg_logfile) {
#if DEV_TARGET
        char err_log[1024] = {0};
        snprintf(err_log, sizeof(err_log),"open file error:%d %s, path:%s", errno, strerror(errno), logfilepath);
        throw err_log;
#else
        __writetips2console("open file error:%d %s, path:%s", errno, strerror(errno), logfilepath);
#endif
    }

    if (0 != s_last_time && (now_time - s_last_time) > (time_t)((now_tick - s_last_tick) / 1000 + 300)) {

        struct tm tm_tmp = *localtime((const time_t*)&s_last_time);
        char last_time_str[64] = {0};
        strftime(last_time_str, sizeof(last_time_str), "%Y-%m-%d %z %H:%M:%S", &tm_tmp);

        tm_tmp = *localtime((const time_t*)&now_time);
        char now_time_str[64] = {0};
        strftime(now_time_str, sizeof(now_time_str), "%Y-%m-%d %z %H:%M:%S", &tm_tmp);

        char log[1024] = {0};
        snprintf(log, sizeof(log), "[F][ last log file:%s from %s to %s, time_diff:%ld, tick_diff:%" PRIu64 "\n", s_last_file_path, last_time_str, now_time_str, now_time-s_last_time, now_tick-s_last_tick);

        AutoBuffer tmp_buff;
        sg_log_buff->Write(log, strnlen(log, sizeof(log)), tmp_buff);
        __writefile(tmp_buff.Ptr(), tmp_buff.Length(), sg_logfile);
    }

    memcpy(s_last_file_path, logfilepath, sizeof(s_last_file_path));
    s_last_tick = now_tick;
    s_last_time = now_time;
    
#ifdef __APPLE__
    assert(sg_logfile);
#endif
    return NULL != sg_logfile;
}

static void __closelogfile() {
    if (NULL == sg_logfile) return;

    sg_openfiletime = 0;
    fclose(sg_logfile);
    sg_logfile = NULL;
}

static void __log2file(const void* _data, size_t _len) {
	if (NULL == _data || 0 == _len || sg_logdir.empty()) {
		return;
	}

	ScopedLock lock_file(sg_mutex_log_file);

	if (sg_cache_logdir.empty()) {
        if (__openlogfile(sg_logdir)) {
            __writefile(_data, _len, sg_logfile);
            
            if (kAppednerAsync == sg_mode) {
                __closelogfile();
            }
            if (__file_size > MAX_SIZE) {
                __cutfile(sg_logdir);
            }
        }
        return;
	}

    struct timeval tv;
    gettimeofday(&tv, NULL);
    char logcachefilepath[1024] = {0};

    __make_logfilename(sg_cache_logdir, sg_logfileprefix.c_str(), LOG_EXT, logcachefilepath, 1024);
    
    if (boost::filesystem::exists(logcachefilepath) && __openlogfile(sg_cache_logdir)) {
        __writefile(_data, _len, sg_logfile);
        if (kAppednerAsync == sg_mode) {
            __closelogfile();
        }

        char logfilepath[1024] = {0};
        __make_logfilename(sg_logdir, sg_logfileprefix.c_str(), LOG_EXT, logfilepath, 1024);
        if (__append_file(logcachefilepath, logfilepath)) {
            if (kAppednerSync == sg_mode) {
                __closelogfile();
            }
            remove(logcachefilepath);
        }
    } else {
        bool write_sucess = false;
        bool open_success = __openlogfile(sg_logdir);
        if (open_success) {
            write_sucess = __writefile(_data, _len, sg_logfile);
            if (kAppednerAsync == sg_mode) {
                __closelogfile();
            }
        }

        if (!write_sucess) {
            if (open_success && kAppednerSync == sg_mode) {
                __closelogfile();
            }

            if (__openlogfile(sg_cache_logdir)) {
                __writefile(_data, _len, sg_logfile);
                if (kAppednerAsync == sg_mode) {
                    __closelogfile();
                }
            }
        }
        if (__file_size > MAX_SIZE && __file_size != UNDEFINED_SIZE) {
            __cutfile(sg_logdir);
        }
    }

}


static void __writetips2file(const char* _tips_format, ...) {

    if (NULL == _tips_format) {
        return;
    }
    
    char tips_info[4096] = {0};
    va_list ap;
    va_start(ap, _tips_format);
    vsnprintf(tips_info, sizeof(tips_info), _tips_format, ap);
    va_end(ap);

    AutoBuffer tmp_buff;
    sg_log_buff->Write(tips_info, strnlen(tips_info, sizeof(tips_info)), tmp_buff);
    
    __log2file(tmp_buff.Ptr(), tmp_buff.Length());
}

static void __async_log_thread() {
    while (true) {

        ScopedLock lock_buffer(sg_mutex_buffer_async);

        if (NULL == sg_log_buff) break;

        AutoBuffer tmp;
        sg_log_buff->Flush(tmp);
        lock_buffer.unlock();

		if (NULL != tmp.Ptr())  __log2file(tmp.Ptr(), tmp.Length());

        if (sg_log_close) break;

        sg_cond_buffer_async.wait(15 * 60 *1000);
    }
}

static void __appender_sync(const XLoggerInfo* _info, const char* _log) {

    char temp[16 * 1024] = {0};     // tell perry,ray if you want modify size.
    PtrBuffer log(temp, 0, sizeof(temp));
    log_formater(_info, _log, log);

    AutoBuffer tmp_buff;
    if (!sg_log_buff->Write(log.Ptr(), log.Length(), tmp_buff))   return;

    __log2file(tmp_buff.Ptr(), tmp_buff.Length());
}

static void __appender_async(const XLoggerInfo* _info, const char* _log) {
    ScopedLock lock(sg_mutex_buffer_async);
    if (NULL == sg_log_buff) return;

    char temp[16*1024] = {0};       //tell perry,ray if you want modify size.
    PtrBuffer log_buff(temp, 0, sizeof(temp));
    log_formater(_info, _log, log_buff);

    if (sg_log_buff->GetData().Length() >= kBufferBlockLength*4/5) {
       int ret = snprintf(temp, sizeof(temp), "[F][ sg_buffer_async.Length() >= BUFFER_BLOCK_LENTH*4/5, len: %d\n", (int)sg_log_buff->GetData().Length());
       log_buff.Length(ret, ret);
    }

    if (!sg_log_buff->Write(log_buff.Ptr(), (unsigned int)log_buff.Length())) return;

    if (sg_log_buff->GetData().Length() >= kBufferBlockLength*1/3 || (NULL!=_info && kLevelFatal == _info->level)) {
       sg_cond_buffer_async.notifyAll();
    }

}


//*
//Bigger file write method
//*
static void __bigger_sync(const char* _log) {
    char temp[16 * 1024] = {0};     // tell perry,ray if you want modify size.
    PtrBuffer log(temp, 0, sizeof(temp));
    log_formater(NULL, _log, log);
    
    AutoBuffer tmp_buff;
    if (!sg_log_buff->Write(log.Ptr(), log.Length(), tmp_buff))   return;
    
    __log2file(tmp_buff.Ptr(), tmp_buff.Length());
}

static void __bigger_async(const char* _log) {
    ScopedLock lock(sg_mutex_buffer_async);
    if (NULL == sg_log_buff) return;
    
    char temp[16*1024] = {0};       //tell perry,ray if you want modify size.
    PtrBuffer log_buff(temp, 0, sizeof(temp));
    log_formater(NULL, _log, log_buff);
    
    if (sg_log_buff->GetData().Length() >= kBufferBlockLength*4/5) {
        int ret = snprintf(temp, sizeof(temp), "[F][ sg_buffer_async.Length() >= BUFFER_BLOCK_LENTH*4/5, len: %d\n", (int)sg_log_buff->GetData().Length());
        log_buff.Length(ret, ret);
    }
    
    if (!sg_log_buff->Write(log_buff.Ptr(), (unsigned int)log_buff.Length())) return;
    
    if (sg_log_buff->GetData().Length() >= kBufferBlockLength*1/3) {
        sg_cond_buffer_async.notifyAll();
    }
}

void bigger_appender(const char* _log) {
    if (sg_log_close) return;
    
    SCOPE_ERRNO();
    
    DEFINE_SCOPERECURSIONLIMIT(recursion);
    static Tss s_recursion_str(free);
    
    if (sg_consolelog_open) __bigger_ConsoleLog(_log);
    
    if (2 <= (int)recursion.Get() && NULL == s_recursion_str.get()) {
        if ((int)recursion.Get() > 10) return;
        char* strrecursion = (char*)calloc(16 * 1024, 1);
        s_recursion_str.set((void*)(strrecursion));
        
        char recursive_log[256] = {0};
        snprintf(recursive_log, sizeof(recursive_log), "ERROR!!! xlogger_appender Recursive calls!!!, count:%d", (int)recursion.Get());
        
        PtrBuffer tmp(strrecursion, 0, 16*1024);
        log_formater(NULL, recursive_log, tmp);
        
        strncat(strrecursion, _log, 4096);
        strrecursion[4095] = '\0';
        
        ConsoleLog(NULL,  strrecursion);
    } else {
        if (NULL != s_recursion_str.get()) {
            char* strrecursion = (char*)s_recursion_str.get();
            s_recursion_str.set(NULL);
            
            __writetips2file(strrecursion);
            free(strrecursion);
        }
        
        if (kAppednerSync == sg_mode)
            __bigger_sync(_log);
        else
            __bigger_async(_log);
    }
}

////////////////////////////////////////////////////////////////////////////////////

void xlogger_appender(const XLoggerInfo* _info, const char* _log) {
    if (sg_log_close) return;

    SCOPE_ERRNO();

    DEFINE_SCOPERECURSIONLIMIT(recursion);
    static Tss s_recursion_str(free);

    if (sg_consolelog_open) ConsoleLog(_info,  _log);

    if (2 <= (int)recursion.Get() && NULL == s_recursion_str.get()) {
        if ((int)recursion.Get() > 10) return;
        char* strrecursion = (char*)calloc(16 * 1024, 1);
        s_recursion_str.set((void*)(strrecursion));

        XLoggerInfo info = *_info;
        info.level = kLevelFatal;

        char recursive_log[256] = {0};
        snprintf(recursive_log, sizeof(recursive_log), "ERROR!!! xlogger_appender Recursive calls!!!, count:%d", (int)recursion.Get());

        PtrBuffer tmp(strrecursion, 0, 16*1024);
        log_formater(&info, recursive_log, tmp);

        strncat(strrecursion, _log, 4096);
        strrecursion[4095] = '\0';

        ConsoleLog(&info,  strrecursion);
    } else {
        if (NULL != s_recursion_str.get()) {
            char* strrecursion = (char*)s_recursion_str.get();
            s_recursion_str.set(NULL);

            __writetips2file(strrecursion);
            free(strrecursion);
        }

        if (kAppednerSync == sg_mode)
            __appender_sync(_info, _log);
        else
            __appender_async(_info, _log);
    }
}

#define HEX_STRING  "0123456789abcdef"
static unsigned int to_string(const void* signature, int len, char* str) {
    char* str_p = str;
    const unsigned char* sig_p;

    for (sig_p = (const unsigned char*) signature;  sig_p - (const unsigned char*)signature < len; sig_p++) {
        char high, low;
        high = *sig_p / 16;
        low = *sig_p % 16;

        *str_p++ = HEX_STRING[(unsigned char)high];
        *str_p++ = HEX_STRING[(unsigned char)low];
        *str_p++ = ' ';
    }

    *str_p++ = '\n';

    for (sig_p = (const unsigned char*) signature;  sig_p - (const unsigned char*)signature < len; sig_p++) {
        *str_p++ = char(isgraph(*sig_p) ? *sig_p : ' ');
        *str_p++ = ' ';
        *str_p++ = ' ';
    }

    return (unsigned int)(str_p - str);
}

const char* xlogger_dump(const void* _dumpbuffer, size_t _len) {
    if (NULL == _dumpbuffer || 0 == _len) {
        //        ASSERT(NULL!=_dumpbuffer);
        //        ASSERT(0!=_len);
        return "";
    }

    SCOPE_ERRNO();

    if (NULL == sg_tss_dumpfile.get()) {
        sg_tss_dumpfile.set(calloc(4096, 1));
    } else {
        memset(sg_tss_dumpfile.get(), 0, 4096);
    }

    ASSERT(NULL != sg_tss_dumpfile.get());

    struct timeval tv = {0};
    gettimeofday(&tv, NULL);
    time_t sec = tv.tv_sec;
    tm tcur = *localtime((const time_t*)&sec);

    char forder_name [128] = {0};
    snprintf(forder_name, sizeof(forder_name), "%d%02d%02d", 1900 + tcur.tm_year, 1 + tcur.tm_mon, tcur.tm_mday);

    std::string filepath =  sg_logdir + "/" + forder_name + "/";

    if (!boost::filesystem::exists(filepath))
        boost::filesystem::create_directory(filepath);
    

    char file_name [128] = {0};
    snprintf(file_name, sizeof(file_name), "%d%02d%02d%02d%02d%02d_%d.dump", 1900 + tcur.tm_year, 1 + tcur.tm_mon, tcur.tm_mday,
             tcur.tm_hour, tcur.tm_min, tcur.tm_sec, (int)_len);
    filepath += file_name;

    FILE* fileid = fopen(filepath.c_str(), "wb");

    if (NULL == fileid) {
        ASSERT2(NULL != fileid, "%s, errno:(%d, %s)", filepath.c_str(), errno, strerror(errno));
        return "";
    }

    fwrite(_dumpbuffer, _len, 1, fileid);
    fclose(fileid);

    char* dump_log = (char*)sg_tss_dumpfile.get();
    dump_log += snprintf(dump_log, 4096, "\n dump file to %s :\n", filepath.c_str());

    int dump_len = 0;

    for (int x = 0; x < 32 && dump_len < (int)_len; ++x) {
        dump_log += to_string((const char*)_dumpbuffer + dump_len, std::min(int(_len) - dump_len, 16), dump_log);
        dump_len += std::min((int)_len - dump_len, 16);
        *(dump_log++) = '\n';
    }

    return (const char*)sg_tss_dumpfile.get();
}

void appender_open(TAppenderMode _mode, const char* _dir, const char* _nameprefix, bool _is_compress, const char* _pub_key) {
    assert(_dir);
    assert(_nameprefix);
    
    if (_pub_key == NULL)
        __is_crypt = false;
    else
        __is_crypt = true;
    
    if (!sg_log_close) {
        __writetips2file("appender has already been opened. _dir:%s _nameprefix:%s", _dir, _nameprefix);
        return;
    }
    
    xlogger_SetAppender(&xlogger_appender);
    
    //mkdir(_dir, S_IRWXU|S_IRWXG|S_IRWXO);
    boost::filesystem::create_directories(_dir);
    tickcount_t tick;
    tick.gettickcount();
    //__del_timeout_file(_dir);
    
    tick.gettickcount();
    
    char mmap_file_path[512] = {0};
    snprintf(mmap_file_path, sizeof(mmap_file_path), "%s/%s.mmap2", sg_cache_logdir.empty()?_dir:sg_cache_logdir.c_str(), _nameprefix);
    
    bool use_mmap = false;
    if (OpenMmapFile(mmap_file_path, kBufferBlockLength, sg_mmmap_file))  {
        sg_log_buff = new LogBuffer(sg_mmmap_file.data(), kBufferBlockLength, _is_compress, _pub_key == NULL ? false : true, _pub_key);
        use_mmap = true;
    } else {
        char* buffer = new char[kBufferBlockLength];
        sg_log_buff = new LogBuffer(buffer, kBufferBlockLength, _is_compress, _pub_key == NULL ? false : true, _pub_key);
        use_mmap = false;
    }
    
    if (NULL == sg_log_buff->GetData().Ptr()) {
        if (use_mmap && sg_mmmap_file.is_open())  CloseMmapFile(sg_mmmap_file);
        return;
    }

    
    AutoBuffer buffer;
    sg_log_buff->Flush(buffer);
    
    ScopedLock lock(sg_mutex_log_file);
    sg_logdir = _dir;
    sg_logfileprefix = _nameprefix;
    sg_log_close = false;
    appender_setmode(_mode);
    lock.unlock();
    
    if (buffer.Ptr()) {
        char kMagicMmapStart = *(char *)buffer.Ptr();
        if ((kMagicMmapStart == kMagicAsyncStart && __is_crypt) || (kMagicMmapStart != kMagicAsyncStart && !__is_crypt))
            __log2file(buffer.Ptr(), buffer.Length());
    }
    
    BOOT_RUN_EXIT(appender_close);
    
}

void appender_open_with_cache(TAppenderMode _mode, const std::string& _cachedir, const std::string& _logdir, const char* _nameprefix, const char* _pub_key) {
    assert(!_cachedir.empty());
    assert(!_logdir.empty());
    assert(_nameprefix);
    
    sg_logdir = _logdir;
    
    if (!_cachedir.empty()) {
        sg_cache_logdir = _cachedir;
        boost::filesystem::create_directories(_cachedir);
        __del_timeout_file(_cachedir);
        // "_nameprefix" must explicitly convert to "std::string", or when the thread is ready to run, "_nameprefix" has been released.
        Thread(boost::bind(&__move_old_files, _cachedir, _logdir, std::string(_nameprefix))).start_after(3 * 60 * 1000);
    }
    
    appender_open(_mode, _logdir.c_str(), _nameprefix, true, _pub_key);
    
}

void appender_flush() {
    sg_cond_buffer_async.notifyAll();
}

void appender_flush_sync() {
    if (kAppednerSync == sg_mode) {
        return;
    }

    ScopedLock lock_buffer(sg_mutex_buffer_async);
    
    if (NULL == sg_log_buff) return;

    AutoBuffer tmp;
    sg_log_buff->Flush(tmp);

    lock_buffer.unlock();

	if (tmp.Ptr())  __log2file(tmp.Ptr(), tmp.Length());

}

void appender_close() {
    if (sg_log_close) return;

    sg_log_close = true;

    sg_cond_buffer_async.notifyAll();

    if (sg_thread_async.isruning())
        sg_thread_async.join();

	
    ScopedLock buffer_lock(sg_mutex_buffer_async);
    if (sg_mmmap_file.is_open()) {
        if (!sg_mmmap_file.operator !()) memset(sg_mmmap_file.data(), 0, kBufferBlockLength);

		CloseMmapFile(sg_mmmap_file);
    } else {
        delete[] (char*)((sg_log_buff->GetData()).Ptr());
    }

    delete sg_log_buff;
    sg_log_buff = NULL;
    buffer_lock.unlock();

    ScopedLock lock(sg_mutex_log_file);
	__closelogfile();
}

void appender_setmode(TAppenderMode _mode) {
    sg_mode = _mode;

    sg_cond_buffer_async.notifyAll();

    if (kAppednerAsync == sg_mode && !sg_thread_async.isruning()) {
        sg_thread_async.start();
    }
}

bool appender_get_current_log_path(char* _log_path, unsigned int _len) {
    if (NULL == _log_path || 0 == _len) return false;

    if (sg_logdir.empty())  return false;

    strncpy(_log_path, sg_logdir.c_str(), _len - 1);
    _log_path[_len - 1] = '\0';
    return true;
}

bool appender_get_current_log_cache_path(char* _logPath, unsigned int _len) {
    if (NULL == _logPath || 0 == _len) return false;
    
    if (sg_cache_logdir.empty())  return false;
    strncpy(_logPath, sg_cache_logdir.c_str(), _len - 1);
    _logPath[_len - 1] = '\0';
    return true;
}

void appender_set_console_log(bool _is_open) {
    sg_consolelog_open = _is_open;
}

void appender_set_max_file_size(uint64_t _max_byte_size) {
    sg_max_file_size = _max_byte_size;
}

void appender_setExtraMSg(const char* _msg, unsigned int _len) {
    sg_log_extra_msg = std::string(_msg, _len);
}


