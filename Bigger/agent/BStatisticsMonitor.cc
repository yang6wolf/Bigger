//
//  BStatisticsMonitor.cpp
//  Bigger
//
//  Created by 王仕杰 on 2017/8/2.
//

#include "BStatisticsMonitor.h"
#include <assert.h>
#include "BAgent.h"
#include "BStatisticsReporter.h"
#include <string.h>

static BStatisticsMonitor* monitor = NULL;

static char identifier[128] = { '\0' };

void bigger_start_realtime_report(int nType, const char *pURL, int nSize, const char **pArrKeys, const char **pArrVals) {
    if (!monitor) {
        monitor = new BStatisticsMonitor();
    }
}
void bigger_end_realtime_report() {
    
}

void setUserIdentifier(const char * iden) {
    assert(strlen(iden) < sizeof(identifier));
    strcpy(identifier, iden);
}

void BStatisticsMonitor::Callback(BLogType eLogType, const char *pLog) {
    assert(eLogType & B_LOG_TYPE_FATAL);
    reportStatisticsMessage(pLog, identifier);
}

void BStatisticsMonitor::regMonitor() {
    BLogDispatcher::RegisterMonitor(this);
}
