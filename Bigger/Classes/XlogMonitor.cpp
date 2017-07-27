//
//  XlogMonitor.cpp
//  Bigger
//
//  Created by 杨志超 on 2017/7/27.
//  Copyright © 2017年 Netease. All rights reserved.
//

#include "XlogMonitor.h"
//#include "LDXLog/xlogger.h"
#include "LDXlog/appender.h"

#import <sys/xattr.h>

const char* XlogMonitor::logDir = NULL;

bool XlogMonitor::isNull() {
    if (logDir == NULL)
        return true;
    return false;
}

void XlogMonitor::Callback(BLogType eLogType, const char *pLog) {
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

void XlogMonitor::init(const char *_logDir, const char *_filePrefix) {
    logDir = _logDir;
    
    // set do not backup for logpath
    const char* attrName = "com.apple.MobileBackup";
    u_int8_t attrValue = 1;
    setxattr(logDir, attrName, &attrValue, sizeof(attrValue), 0, 0);
    
#if DEBUG
    //xlogger_SetLevel(kLevelDebug);
    appender_set_console_log(true);
#else
    //xlogger_SetLevel(kLevelInfo);
    appender_set_console_log(false);
#endif
    open(_logDir, _filePrefix);
}

void XlogMonitor::open(const char *_logDir, const char *_filePrefix) {
    appender_open(kAppednerAsync, _logDir, _filePrefix);
}

void XlogMonitor::close() {
    appender_close();
}

void XlogMonitor::asyncFlush() {
    appender_flush();
}

void XlogMonitor::syncFlush() {
    appender_flush_sync();
}
