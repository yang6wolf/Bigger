//
//  XlogMonitor.cpp
//  Bigger
//
//  Created by 杨志超 on 2017/7/27.
//  Copyright © 2017年 Netease. All rights reserved.
//

#include "BigEyesMonitor.h"
#include "LDXlog/appender.h"

#import <sys/xattr.h>

const char* BigEyesMonitor::logPath = NULL;
const char* BigEyesMonitor::prefix = NULL;
bool BigEyesMonitor::isRegister = false;

bool BigEyesMonitor::isPathNull() {
    if (logPath == NULL)
        return true;
    return false;
}

void BigEyesMonitor::Callback(BLogType eLogType, const char *pLog) {
    switch (eLogType) {
        case B_LOG_TYPE_ERROR:
            bigger_appender(pLog);
            flush();
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
    printf("prefix : %s", prefix);
}

void BigEyesMonitor::init(const char *_logPath, const char *_prefix) {
    logPath = _logPath;
    prefix = _prefix;
    
    // set do not backup for logpath
    const char* attrName = "com.apple.MobileBackup";
    u_int8_t attrValue = 1;
    setxattr(logPath, attrName, &attrValue, sizeof(attrValue), 0, 0);
    
    appender_set_console_log(false);
}

void BigEyesMonitor::open() {
    appender_open(kAppednerAsync, logPath, prefix, NULL);
}

void BigEyesMonitor::close() {
    appender_close();
}

void BigEyesMonitor::flush() {
    appender_flush();
}

void BigEyesMonitor::syncFlush() {
    appender_flush_sync();
}

const char* BigEyesMonitor::getPath() {
    return logPath;
}
