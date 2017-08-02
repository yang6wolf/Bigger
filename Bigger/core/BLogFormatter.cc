//
//  BLogFormatter.cc
//  Pods
//
//  Created by Edward on 31/7/17.
//
//

#include "BLogFormatter.h"

#include <string.h>
#include <stddef.h>
#include <assert.h>
#include <stdio.h>
#include <limits.h>
#include <algorithm>

const char *ExtractFileName(const char *_path) {
    if (NULL == _path) return "";
    
    const char *pos = strrchr(_path, '\\');
    
    if (NULL == pos) {
        pos = strrchr(_path, '/');
    }
    
    if (NULL == pos || '\0' == *(pos + 1)) {
        return _path;
    } else {
        return pos + 1;
    }
}

void ExtractFunctionName(const char *_func, char *_func_ret, int _len) {
    if (NULL == _func)return;
    
    const char *start = _func;
    const char *end = NULL;
    const char *pos = _func;
    
    while ('\0' != *pos) {
        if (NULL == end && ' ' == *pos) {
            start = ++pos;
            continue;
        }
        
        if (':' == *pos && ':' == *(pos+1)) {
            pos += 2;
            start = pos;
            continue;
        }
        
        if ('(' == *pos) {
            end = pos;
        } else if (NULL != start && (':' == *pos || ']' == *pos)) {
            end = pos;
            break;
        }
        ++pos;
    }
    
    
    if (NULL == start || NULL == end || start + 1 >= end) {
        strncpy(_func_ret, _func, _len);
        _func_ret[_len - 1] = '\0';
        return;
    }
    
    ptrdiff_t len = end - start;
    --_len;
    len = _len < len ? _len : len;
    memcpy(_func_ret, start, len);
    _func_ret[len] = '\0';
}

void formatLogHeader(char *pBuf, BLogType eLogType, intmax_t nPID, intmax_t nTID, intmax_t nMainTID, const char *pTag, const char *pFileName, int nLineNumber, const char *pFuncName, struct timeval *pTimeval, const char *pBody) {
    
    static const char *pLevel=NULL;
    switch (eLogType) {
        case B_LOG_TYPE_DEBUG:
            pLevel="D";
            break;
        case B_LOG_TYPE_INFO:
            pLevel="I";
            break;
        case B_LOG_TYPE_ERROR:
            pLevel="E";
            break;
        default:
            pLevel="E";
            break;
    }
    
    const char *filename = ExtractFileName(pFileName);
    char strFuncName [128] = {0};
    ExtractFunctionName(pFuncName, strFuncName, sizeof(strFuncName));
    
    char temp_time[64] = {0};
    
    if (0 != pTimeval->tv_sec) {
        time_t sec = pTimeval->tv_sec;
        tm tm = *localtime((const time_t*)&sec);
#ifdef ANDROID
        snprintf(temp_time, sizeof(temp_time), "%d-%02d-%02d %02d:%02d:%02d.%.3ld", 1900 + tm.tm_year, 1 + tm.tm_mon, tm.tm_mday,
                  tm.tm_hour, tm.tm_min, tm.tm_sec, pTimeval->tv_usec / 1000);
#elif _WIN32
        snprintf(temp_time, sizeof(temp_time), "%d-%02d-%02d %02d:%02d:%02d.%.3d", 1900 + tm.tm_year, 1 + tm.tm_mon, tm.tm_mday,
                  tm.tm_hour, tm.tm_min, tm.tm_sec, pTimeval->tv_usec / 1000);
#else
        snprintf(temp_time, sizeof(temp_time), "%d-%02d-%02d %02d:%02d:%02d.%.3d", 1900 + tm.tm_year, 1 + tm.tm_mon, tm.tm_mday,
                  tm.tm_hour, tm.tm_min, tm.tm_sec, pTimeval->tv_usec / 1000);
#endif
    }
    
    snprintf(pBuf, 1024, "[%s][%" PRIdMAX ":%02" PRIdMAX "%s][%s][%s][%s, %s, %03d] %s",
                       temp_time,
             nPID, nTID, nTID == nMainTID ? "*" : "", pLevel, pTag ? pTag : "",
                       filename, strFuncName, nLineNumber, pBody);
}
