//
//  BLogger.h
//  Bigger
//
//  Created by Edward on 11/7/17.
//  Copyright © 2017年 NetEase. All rights reserved.
//

#ifndef _BLOGGER_H_
#define _BLOGGER_H_

#include "BCommonDef.h"

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
    static int RegisterMonitor(BLogMonitor *pMonitor);
    
    static int DeReisterMonitor(BLogMonitor *pMonitor);
    
protected:
    BLogDispatcher();
    ~BLogDispatcher();
}BLogDispatcher;



#endif /* _BLOGGER_H_ */
