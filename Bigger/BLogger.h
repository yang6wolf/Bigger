//
//  BLogger.h
//  Bigger
//
//  Created by Edward on 11/7/17.
//  Copyright © 2017年 Netease. All rights reserved.
//

#ifndef _BLOGGER_H_
#define _BLOGGER_H_

#include <stdio.h>


//typedef enum BLogType{
//    B_LOG_TYPE_ERROR=1,
//    B_LOG_TYPE_INFO=1<<4,
//    B_LOG_TYPE_DEBUG=1<<8
//}BLogType;


//#define LOGD(fmt, ...) BLogDispatcher::WriteLog(B_LOG_TYPE_DEBUG, fmt, ##__VA_ARGS__)
//#define LOGI(fmt, ...) BLogDispatcher::WriteLog(B_LOG_TYPE_INFO, fmt, ##__VA_ARGS__)
//#define LOGE(fmt, ...) BLogDispatcher::WriteLog(B_LOG_TYPE_ERROR, fmt, ##__VA_ARGS__)

#include "BLoggerInterface.h"

typedef class BLogMonitor {
public:
    virtual void Callback(BLogType eLogType, const char *pLog) = 0;
    
    int _MonitorType=0;
    
protected:
    virtual ~BLogMonitor() {}
    
    friend class BLogDispatcher;
    int _MonitorID;
}BLogMonitor;



typedef class BLogDispatcher {
public:
    
    static void WriteLog(BLogType eLogType, const char *pLog);
    
    // if want to monitor multi types, use | to combine them
    static int RegisterMonitor(BLogMonitor &observer);
    
    static int DeReisterMonitor(BLogMonitor &observer);
    
protected:
    BLogDispatcher();
    ~BLogDispatcher();
}BLogDispatcher;



#endif /* _BLOGGER_H_ */
