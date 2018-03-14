//
//  BStatisticsMonitor.hpp
//  Bigger
//
//  Created by 王仕杰 on 2017/8/2.
//

#ifndef BStatisticsMonitor_hpp
#define BStatisticsMonitor_hpp

#include <stdio.h>
#include "BLogger.h"

class BStatisticsMonitor : public BLogMonitor {
public:
    void Callback(BLogType eLogType, const char *pLog);
    
    BStatisticsMonitor() {
        _MonitorType = B_LOG_TYPE_FATAL | B_LOG_TYPE_ERROR | B_LOG_TYPE_INFO;
        regMonitor();
    }
private:
    void regMonitor();
};

#endif /* BStatisticsMonitor_hpp */
