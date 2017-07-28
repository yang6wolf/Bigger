//
//  XlogMonitor.cpp
//  Bigger
//
//  Created by 杨志超 on 2017/7/27.
//  Copyright © 2017年 Netease. All rights reserved.
//

#include "BigWriterMonitor.h"
#include "LDXlog/appender.h"

#import <sys/xattr.h>

const char* BigWriterMonitor::logPath = NULL;
bool BigWriterMonitor::isRegister = false;

bool BigWriterMonitor::isPathNull() {
    if (logPath == NULL)
        return true;
    return false;
}

void BigWriterMonitor::Callback(BLogType eLogType, const char *pLog) {
    switch (eLogType) {
        case B_LOG_TYPE_ERROR:
            bigger_appender(pLog);
        break;
        
        case B_LOG_TYPE_INFO:
            bigger_appender(pLog);
        break;
        
        case B_LOG_TYPE_DEBUG:
            bigger_appender(pLog);
        break;
        
        default:
        break;
    }
    printf("log : %s", pLog);
}

void BigWriterMonitor::init(const char *_logPath, const char *_filePrefix) {
    logPath = _logPath;
    
    // set do not backup for logpath
    const char* attrName = "com.apple.MobileBackup";
    u_int8_t attrValue = 1;
    setxattr(logPath, attrName, &attrValue, sizeof(attrValue), 0, 0);
    
#if DEBUG
    appender_set_console_log(true);
#else
    appender_set_console_log(false);
#endif
}

void BigWriterMonitor::open(const char *_logPath, const char *_filePrefix) {
    appender_open(kAppednerAsync, _logPath, _filePrefix);
}

void BigWriterMonitor::close() {
    appender_close();
}

void BigWriterMonitor::asyncFlush() {
    appender_flush();
}

void BigWriterMonitor::syncFlush() {
    appender_flush_sync();
}

