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

#ifndef APPENDER_H_
#define APPENDER_H_

#include <string>
#include <vector>
#include <stdint.h>
#include <stdlib.h>
#include "mutex.h"
#include "condition.h"
#include "log_buffer.h"
#include "thread.h"

#include "boost/iostreams/device/mapped_file.hpp"
#include "comm/xlogger/xloggerbase.h"

#define UNDEFINED_SIZE 2000000000
#define MAX_SIZE 5242880
#define MIN_SIZE 3145728

enum TAppenderMode
{
    kAppednerAsync,
    kAppednerSync,
};

class Appender {
public:
    Appender() {
    }
    void appender_open(TAppenderMode _mode, const char* _dir, const char* _nameprefix, bool _is_compress, const char* _pub_key);
    
    /* no implementation
    void appender_open_with_cache(TAppenderMode _mode, const std::string& _cachedir, const std::string& _logdir, const char* _nameprefix, const char* _pub_key);
     */
    void appender_flush();
    void appender_flush_sync();
    void appender_close();
    void appender_setmode(TAppenderMode _mode);
    bool appender_get_current_log_path(char* _log_path, unsigned int _len);
    bool appender_get_current_log_cache_path(char* _logPath, unsigned int _len);
    void appender_set_console_log(bool _is_open);
    
    void bigger_appender(const char* _log);
    void bigger_flush();
    /*
     * By default, all logs will write to one file everyday. You can split logs to multi-file by changing max_file_size.
     *
     * @param _max_byte_size    Max byte size of single log file, default is 0, meaning do not split.
     */
    void appender_set_max_file_size(uint64_t _max_byte_size);
    std::string sg_logfileprefix;
    
private:
    void appender_setExtraMSg(const char* _msg, unsigned int _len);
    void __move_old_files(const std::string& _src_path, const std::string& _dest_path, const std::string& _nameprefix);
    void __cutfile(const std::string& _log_dir);
    bool __writefile(const void* _data, size_t _len, FILE* _file);
    bool __openlogfile(const std::string& _log_dir);
    void __closelogfile();
    void __log2file(const void* _data, size_t _len);
    void __writetips2file(const char* _tips_format, ...);
    void __async_log_thread();
    void __appender_sync(const XLoggerInfo* _info, const char* _log);
    void __appender_async(const XLoggerInfo* _info, const char* _log);
    void __bigger_sync(const char* _log);
    void __bigger_async(const char* _log);
    
    void xlogger_appender(const XLoggerInfo* _info, const char* _log);
    const char* xlogger_dump(const void* _dumpbuffer, size_t _len);
    
    TAppenderMode sg_mode = kAppednerAsync;
    
    Thread* sg_thread_async;
    
    std::string sg_logdir;
    std::string sg_cache_logdir;
    void (^__async_log_thread_b)(void);
    
    Mutex sg_mutex_buffer_async;
    Mutex sg_mutex_log_file;
    FILE* sg_logfile = NULL;
    time_t sg_openfiletime = 0;
    std::string sg_current_dir;
    
#ifdef _WIN32
    Condition& sg_cond_buffer_async = *(new Condition());  // 改成引用, 避免在全局释放时执行析构导致crash
#else
    Condition sg_cond_buffer_async;
#endif
    
    LogBuffer* sg_log_buff = NULL;
    
    bool sg_log_close = true;

#ifdef DEBUG
    bool sg_consolelog_open = true;
#else
    bool sg_consolelog_open = false;
#endif
    
    uint64_t sg_max_file_size = 0; // 0, will not split log file.
    
    std::string sg_log_extra_msg;
    
    boost::iostreams::mapped_file sg_mmmap_file;
    
    bool __is_crypt = false;
    uintmax_t __file_size = UNDEFINED_SIZE;
    const char kMagicAsyncStart ='\x07';
    char* __logdata_temp = NULL;
};



#endif /* APPENDER_H_ */
