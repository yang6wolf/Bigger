//
//  XlogMonitor.cpp
//  Bigger
//
//  Created by 杨志超 on 2017/7/27.
//  Copyright © 2017年 Netease. All rights reserved.
//

#include "BFileWriter.h"
#include "LDXlog/appender.h"

#import <sys/xattr.h>

const char* BigWriter::logPath = NULL;
const char* BigWriter::prefix = NULL;
bool BigWriter::isRegister = false;
bool BigWriter::isCompress = false;
bool BigWriter::isCrypt = false;

bool BigWriter::isPathNull() {
    if (logPath == NULL)
        return true;
    return false;
}

void BigWriter::Callback(BLogType eLogType, const char *pLog) {
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
}

void BigWriter::init(const char *_logPath, const char *_prefix, bool _isCompress, bool _isCrypt) {
    logPath = _logPath;
    prefix = _prefix;
    isCompress = _isCompress;
    isCrypt = _isCrypt;
    
    // set do not backup for logpath
    const char* attrName = "com.apple.MobileBackup";
    u_int8_t attrValue = 1;
    setxattr(logPath, attrName, &attrValue, sizeof(attrValue), 0, 0);
    
    appender_set_console_log(false);
}

void BigWriter::open() {
    appender_open(kAppednerAsync, logPath, prefix, isCompress, isCrypt, NULL);
}

void BigWriter::close() {
    appender_close();
}

void BigWriter::flush() {
    appender_flush();
}

void BigWriter::syncFlush() {
    appender_flush_sync();
}

const char* BigWriter::getPath() {
    return logPath;
}
