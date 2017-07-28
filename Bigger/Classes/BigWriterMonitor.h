//
//  XlogMonitor.hpp
//  Bigger
//
//  Created by 杨志超 on 2017/7/27.
//  Copyright © 2017年 Netease. All rights reserved.
//

#ifndef BigWriterMonitor_h
#define BigWriterMonitor_h

#include <stdio.h>
#include "BLogger.h"

class BigWriterMonitor : public BLogMonitor {
public:
    void Callback(BLogType eLogType, const char *pLog);
    
    virtual void init(const char *_logPath, const char *_filePrefix);
    virtual void open(const char *_logPath, const char *_filePrefix);
    virtual void close();
    virtual void asyncFlush();
    virtual void syncFlush();
    
    bool isPathNull();
    
    void setRegister(bool _isRegister) {isRegister = _isRegister;};
    bool getRegister() {return isRegister;};
    
    BigWriterMonitor() {
        //logPath = NULL;
        isRegister = false;
        _MonitorType = B_LOG_TYPE_ERROR | B_LOG_TYPE_INFO | B_LOG_TYPE_DEBUG;
    }
    
    virtual ~BigWriterMonitor() {
        delete logPath;
    }
private:
    static const char *logPath;
    static bool isRegister;
};
#endif /* BigWriterMonitor_h */
