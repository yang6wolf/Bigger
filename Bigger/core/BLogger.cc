//
//  BLogger.cc
//  Bigger
//
//  Created by Edward on 11/7/17.
//  Copyright © 2017年 NetEase. All rights reserved.
//

#include "BLogger.h"
#include "BAgent.h"

#include <vector>
#include <mutex>
#include <assert.h>

#include <future>

using namespace std;

vector<BLogMonitor *> vecMonitors;
mutex mutex4Monitor;

void BLogDispatcher::WriteLog(BLogType eLogType, const char *pLog) {
    mutex4Monitor.lock();
    vector<BLogMonitor *>::iterator it;
    for (it=vecMonitors.begin(); it!=vecMonitors.end(); it++) {
        if ((*it)->_MonitorType&eLogType) {
            (*it)->Callback(eLogType, pLog);
        }
    }
    mutex4Monitor.unlock();
    
    return;
}

int BLogDispatcher::RegisterMonitor(BLogMonitor *pMonitor) {
    mutex4Monitor.lock();
    vecMonitors.push_back(pMonitor);
    mutex4Monitor.unlock();
    
    return 1;
}

int BLogDispatcher::DeReisterMonitor(BLogMonitor *pMonitor) {
    mutex4Monitor.lock();
    vector<BLogMonitor *>::const_iterator it;
    for (it=vecMonitors.begin(); it!=vecMonitors.end();) {
        if ((*it)->_MonitorID == pMonitor->_MonitorID) {
            it=vecMonitors.erase(it);
        }
        else {
            it++;
        }
    }
    mutex4Monitor.unlock();
    
    return 1;
}

void SwiftBiggerLog(BLogType type, const char * log) {
    std::async(std::launch::async, BLogDispatcher::WriteLog, type, log);
}

