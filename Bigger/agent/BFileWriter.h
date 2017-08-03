//
//  XlogMonitor.hpp
//  Bigger
//
//  Created by 杨志超 on 2017/7/27.
//  Copyright © 2017年 Netease. All rights reserved.
//

#ifndef BigEyesMonitor_h
#define BigEyesMonitor_h

#include <stdio.h>
#include "BLogger.h"

class BigWriter : public BLogMonitor {
public:
    void Callback(BLogType eLogType, const char *pLog);
    
    void init(const char *_logPath, const char *_prefix, bool isCompress, bool isCrypt);
    void open();
    void close();
    void flush();
    void syncFlush();
    
    const char* getPath();
    bool isPathNull();
    
    void setMonitorType(int _type) {_MonitorType = _type;};
    void setRegister(bool _isRegister) {isRegister = _isRegister;};
    bool getRegister() {return isRegister;};
    
    BigWriter() {
        _MonitorType = B_LOG_TYPE_ERROR | B_LOG_TYPE_INFO | B_LOG_TYPE_DEBUG;
    }
    
    virtual ~BigWriter() {
    }
private:
    static const char *logPath;
    static const char *prefix;
    static bool isRegister;
    static bool isCompress;
    static bool isCrypt;
};
#endif /* BigEyesMonitor_h */
