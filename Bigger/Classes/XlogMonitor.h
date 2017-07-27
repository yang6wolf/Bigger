//
//  XlogMonitor.hpp
//  Bigger
//
//  Created by 杨志超 on 2017/7/27.
//  Copyright © 2017年 Netease. All rights reserved.
//

#ifndef XlogMonitor_h
#define XlogMonitor_h

#include <stdio.h>
#include "BLogger.h"

class XlogMonitor : public BLogMonitor {
public:
    void Callback(BLogType eLogType, const char *pLog);
    
    virtual void init(const char *_logDir, const char *_filePrefix);
    virtual void open(const char *_logDir, const char *_filePrefix);
    virtual void close();
    virtual void asyncFlush();
    virtual void syncFlush();
    
    bool isNull();
    
    XlogMonitor() {
        //logDir = NULL;
        _MonitorType = B_LOG_TYPE_ERROR | B_LOG_TYPE_INFO | B_LOG_TYPE_DEBUG;
    }
    
    ~XlogMonitor() {
        delete logDir;
    }
private:
    static const char *logDir;
    
};
#endif /* XlogMonitor_h */
