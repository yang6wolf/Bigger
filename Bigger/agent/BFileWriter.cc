//
//  BFileWriter.cc
//  Bigger
//
//  Created by 杨志超 on 2017/7/27.
//  Copyright © 2017年 NetEase. All rights reserved.
//

#include "BFileWriter.h"
#include "BAgentInternal.h"

#import <sys/xattr.h>
#import <sstream>

bool BigWriter::isPathNull() {
    if (logPath.empty())
        return true;
    return false;
}

void BigWriter::Callback(BLogType eLogType, const char *pLog) {
    std::stringstream streamLog;
    streamLog << "[" << strAppID << "][" << strDeviceID << "]" << pLog;
    
    switch (eLogType) {
        case B_LOG_TYPE_FATAL:
            appender -> bigger_appender(streamLog.str().c_str());
            flush();
        break;
        
        case B_LOG_TYPE_ERROR:
            appender -> bigger_appender(streamLog.str().c_str());
        break;
        
        case B_LOG_TYPE_INFO:
            appender -> bigger_appender(streamLog.str().c_str());
        break;
        
        case B_LOG_TYPE_DEBUG:
            appender -> bigger_appender(streamLog.str().c_str());
        break;
        
        default:
        break;
    }
}

void BigWriter::init(const char *_logPath, bool _isCompress, bool _isCrypt) {
    logPath = _logPath;
    isCompress = _isCompress;
    isCrypt = _isCrypt;
    
    // set do not backup for logpath
    const char* attrName = "com.apple.MobileBackup";
    u_int8_t attrValue = 1;
    setxattr(logPath.c_str(), attrName, &attrValue, sizeof(attrValue), 0, 0);
}

void BigWriter::open(const char * filename) {
    static const char *pub_key = "c6909c84d9099761b8e7dd91615492b81fc1f81df563ad208564ba304d94f0146d10d7cc163de76fa8ecc3c0696444e855ec0fa68c05f00c84b49e18abe67c19";
    appender = new Appender();
    appender -> appender_open(kAppednerAsync, logPath.c_str(), filename, isCompress, isCrypt == false ? NULL : pub_key);
}

void BigWriter::close() {
    appender -> appender_close();
}

void BigWriter::flush() {
    if (isRegister)
        appender -> appender_flush();
}

void BigWriter::syncFlush() {
    if (isRegister)
        appender -> appender_flush_sync();
}

const char* BigWriter::getPath() {
    return logPath.c_str();
}

const char* BigWriter::getFilename() {
    return appender -> sg_logfileprefix.c_str();
}
