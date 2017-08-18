//
//  XlogMonitor.cpp
//  Bigger
//
//  Created by 杨志超 on 2017/7/27.
//  Copyright © 2017年 NetEase. All rights reserved.
//

#include "BFileWriter.h"
#include "LDXlog/appender.h"

#import <sys/xattr.h>

char* BigWriter::logPath = NULL;
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
        case B_LOG_TYPE_FATAL:
            bigger_appender(pLog);
            flush();
        break;
        
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
}

void BigWriter::init(const char *_logPath, bool _isCompress, bool _isCrypt) {
    if (logPath == NULL) {
        logPath = new char[strlen(_logPath)];
        memcpy(logPath, _logPath, strlen(_logPath));
    }
    isCompress = _isCompress;
    isCrypt = _isCrypt;
    
    // set do not backup for logpath
    const char* attrName = "com.apple.MobileBackup";
    u_int8_t attrValue = 1;
    setxattr(logPath, attrName, &attrValue, sizeof(attrValue), 0, 0);
    
    appender_set_console_log(false);
}

void BigWriter::open() {
    const char *pub_key = "c6909c84d9099761b8e7dd91615492b81fc1f81df563ad208564ba304d94f0146d10d7cc163de76fa8ecc3c0696444e855ec0fa68c05f00c84b49e18abe67c19";
    appender_open(kAppednerAsync, logPath, "dailylog", isCompress, isCrypt == false ? NULL : pub_key);
}

void BigWriter::close() {
    appender_close();
}

void BigWriter::flush() {
    if (isRegister)
        appender_flush();
}

void BigWriter::syncFlush() {
    if (isRegister)
        appender_flush_sync();
}

const char* BigWriter::getPath() {
    return logPath;
}
