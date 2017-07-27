//
//  XlogInterface.cpp
//  Pods
//
//  Created by 杨志超 on 2017/7/27.
//
//

#include "XlogInterface.h"
#include "XlogMonitor.h"

static XlogMonitor *xlogMonitor = NULL;

void initXlog(const char *_logDir, const char *_filePrefix) {
    if (xlogMonitor == NULL)
        xlogMonitor = new XlogMonitor();
    xlogMonitor->init(_logDir, _filePrefix);
    BLogDispatcher::RegisterMonitor(*xlogMonitor);
}

void openXlog(const char *_logDir, const char *_filePrefix) {
    if (xlogMonitor == NULL)
        xlogMonitor = new XlogMonitor();
    xlogMonitor->open(_logDir, _filePrefix);
}

void closeXlog() {
    xlogMonitor->close();
    delete xlogMonitor;
    xlogMonitor = NULL;
}



