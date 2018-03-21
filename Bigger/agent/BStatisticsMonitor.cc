//
//  BStatisticsMonitor.cpp
//  Bigger
//
//  Created by 王仕杰 on 2017/8/2.
//

#include "BStatisticsMonitor.h"
#include <assert.h>
#include "BAgentInternal.h"
#include "BStatisticsReporter.h"
#include <string.h>
#include <sstream>

#import "curl/curl.h"

static BStatisticsMonitor* monitor = NULL;

static char identifier[128] = { '\0' };

bool bigger_start_realtime_report(int nType, const char *pURL, int nSize, const char **pArrKeys, const char **pArrVals) {
    if (strAppID.empty() || strDeviceID.empty()) {
        printf("bigger_start_realtime_report error!\n");
        return false;
    }
    
    if (!monitor) {
        monitor = new BStatisticsMonitor();
        curl_global_init(CURL_GLOBAL_ALL);
    }
    return true;
}
void bigger_end_realtime_report() {
    curl_global_cleanup();
}

void setUserIdentifier(const char * iden) {
    assert(strlen(iden) < sizeof(identifier));
    strcpy(identifier, iden);
}

void BStatisticsMonitor::Callback(BLogType eLogType, const char *pLog) {
    
    std::stringstream streamLog;
    streamLog << "[" << strAppID << "][" << strDeviceID << "]" << pLog;
    reportStatisticsMessage(streamLog.str().c_str(), identifier);
}

void BStatisticsMonitor::regMonitor() {
    BLogDispatcher::RegisterMonitor(this);
}
