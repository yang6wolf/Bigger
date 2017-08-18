//
//  XlogMonitor.hpp
//  Bigger
//
//  Created by 杨志超 on 2017/7/27.
//  Copyright © 2017年 NetEase. All rights reserved.
//

#ifndef BigWriter_h
#define BigWriter_h

#include <stdio.h>
#include "BLogger.h"

class BigWriter : public BLogMonitor {
public:
    void Callback(BLogType eLogType, const char *pLog);
    
    void init(const char *_logPath, bool isCompress, bool isCrypt);
    void open();
    void close();
    void flush();
    void syncFlush();
    
    const char* getPath();
    bool isPathNull();
    
    void setMonitorType(int _type) {_MonitorType = _type;};
    void setRegister(bool _isRegister) {isRegister = _isRegister;};
    bool getRegister() {return isRegister;};
    bool getCrypt() {return isCrypt;};
    bool getCompress() {return isCompress;};
    
    BigWriter() {
        _MonitorType = B_LOG_TYPE_FATAL | B_LOG_TYPE_ERROR | B_LOG_TYPE_INFO | B_LOG_TYPE_DEBUG;
    }
    
    virtual ~BigWriter() {
    }
private:
    static char *logPath;
    static bool isRegister;
    static bool isCompress;
    static bool isCrypt;
};
#endif /* BigWriter_h */
