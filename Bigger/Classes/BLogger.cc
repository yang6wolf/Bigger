//
//  BLogger.cc
//  Bigger
//
//  Created by Edward on 11/7/17.
//  Copyright © 2017年 Netease. All rights reserved.
//

#include "BLogger.h"

#include <vector>
using namespace std;

vector<BLogMonitor *> vecMonitors;



void BLogDispatcher::WriteLog(BLogType eLogType, const char *pLog) {
    vector<BLogMonitor *>::iterator it;
    for (it=vecMonitors.begin(); it!=vecMonitors.end(); it++) {
        if ((*it)->_MonitorType&eLogType) {
            (*it)->Callback(eLogType, pLog);
        }
    }
    
    return;
}

int BLogDispatcher::RegisterMonitor(BLogMonitor *pMonitor) {
    vecMonitors.push_back(pMonitor);
    return 1;
}

int BLogDispatcher::DeReisterMonitor(BLogMonitor *pMonitor) {
    vector<BLogMonitor *>::const_iterator it;
    for (it=vecMonitors.begin(); it!=vecMonitors.end();) {
        if ((*it)->_MonitorID == pMonitor->_MonitorID) {
            it=vecMonitors.erase(it);
        }
        else {
            it++;
        }
    }
    
    return 1;
}

