//
//  BStatisticsMonitor.cpp
//  Bigger
//
//  Created by 王仕杰 on 2017/8/2.
//

#include "BStatisticsMonitor.h"
#include "BAgentInternal.h"
#include "BStatisticsReporter.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sstream>
#include <vector>
#include <future>

std::vector<BStatisticsMonitor *> global_stat_monitor;

bool bigger_start_realtime_report(int nType, const char *pURL, int h_size, const char * headers[], LogFormatter formatter) {

    global_stat_monitor.push_back(new BStatisticsMonitor(nType, pURL, h_size, headers, formatter));
    
    return false;
}

void bigger_end_realtime_report(const char *pURL) {
    for (auto it = global_stat_monitor.begin(); it != global_stat_monitor.end(); ) {
        if (!strcmp((*it)->getURL(), pURL)) {
            it = global_stat_monitor.erase(it);
            delete *it;
        } else {
            it++;
        }
    }
}

void BStatisticsMonitor::Callback(BLogType eLogType, const char *pLog) {
    //暂时不引入fmt函数及其逻辑
    std::stringstream streamLog;
    streamLog << "[" << strAppID << "][" << strDeviceID << "]" << pLog;
    char * msg = strdup(streamLog.str().c_str());
    std::thread t(report_statistics_msg, msg, url, headerSize, headerField);
    t.detach();
}

void BStatisticsMonitor::regMonitor() {
    _MonitorType = monitorType;
    BLogDispatcher::RegisterMonitor(this);
}

const char * BStatisticsMonitor::getURL() {
    return this -> url;
}

void BStatisticsMonitor::copyStrings(const char * dest[], const char * source [], int size) {
    for (int i = 0; i < size; i++) {
        dest[i] = source[i];
    }
}

