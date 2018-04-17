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
#include "BAgent.h"
#include "string.h"
#include "stdlib.h"
#include "time.h"

class BStatisticsMonitor : public BLogMonitor {
public:
    void Callback(BLogType eLogType, const char *pLog);
    
    BStatisticsMonitor(int t, const char * u, int s, const char ** h, LogFormatter f) {
        monitorType = t;
        url = strdup(u);
        headerSize = s;
        headerField = (const char **)malloc(s * sizeof(char *));
        copyStrings(headerField, h, s);
        fmt = f;
        _MonitorID = arc4random();
        regMonitor();
    }
    
    ~BStatisticsMonitor() {
        BLogDispatcher::DeReisterMonitor(this);
        delete url;
        delete [] headerField;
    }
    
    const char * getURL();
    
private:
    void regMonitor();
    int monitorType;
    int headerSize;
    const char * url;
    const char ** headerField;
    LogFormatter fmt;
    
    void copyStrings(const char **dest, const char **source, int size);
};

#endif /* BStatisticsMonitor_hpp */
