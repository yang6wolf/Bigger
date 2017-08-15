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

static BStatisticsMonitor* monitor = NULL;

void initStatisticsMonitor() {
    if (!monitor) {
        monitor = new BStatisticsMonitor();
    }
}

void BStatisticsMonitor::Callback(BLogType eLogType, const char *pLog) {
    assert(eLogType & B_LOG_TYPE_FATAL);
    reportStatisticsMessage(pLog);
}

void BStatisticsMonitor::regMonitor() {
    BLogDispatcher::RegisterMonitor(this);
}
