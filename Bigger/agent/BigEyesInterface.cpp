//
//  XlogInterface.cpp
//  Pods
//
//  Created by 杨志超 on 2017/7/27.
//
//

#include "BAgent.h"
#include "BigEyesMonitor.h"

static BigEyesMonitor *bigEyesMonitor = NULL;

void initBigEyes(const char *_logPath, const char *_prefix, bool _isCrypt) {
    if (bigEyesMonitor == NULL) {
        bigEyesMonitor = new BigEyesMonitor();
        bigEyesMonitor->setMonitorType(B_LOG_TYPE_ERROR | B_LOG_TYPE_INFO | B_LOG_TYPE_DEBUG);
    }
    
    if (bigEyesMonitor->isPathNull())
        bigEyesMonitor->init(_logPath, _prefix);
    else {
        printf("BigEyesMonitor has already been inited!\n");
        return;
    }
    
    openBigEyes(_isCrypt);
}

void initBigEyesWithType(const char *_logPath, const char *_prefix, bool _isCrypt, int nType) {
    if (bigEyesMonitor == NULL) {
        bigEyesMonitor = new BigEyesMonitor();
        bigEyesMonitor->setMonitorType(nType);
    }
    
    if (bigEyesMonitor->isPathNull())
    bigEyesMonitor->init(_logPath, _prefix);
    else {
        printf("BigEyesMonitor has already been inited!\n");
        return;
    }
    
    openBigEyes(_isCrypt);
}

bool checkInit() {
    if (bigEyesMonitor == NULL || bigEyesMonitor->isPathNull()) {
        printf("BigEyes didn't init!");
        return false;
    }
    return true;
}

void openBigEyesWithType(bool _isCrypt, int nType) {
    if (!checkInit())
        return;
    bigEyesMonitor->setMonitorType(nType);
    openBigEyes(_isCrypt);
}

void openBigEyes(bool _isCrypt) {
    if (!checkInit())
        return;
    if (!bigEyesMonitor->getRegister()) {
        bigEyesMonitor->open();
        BLogDispatcher::RegisterMonitor(bigEyesMonitor);
        bigEyesMonitor->setRegister(true);
    }
}

void closeBigEyes() {
    if (!checkInit())
        return;
    if (bigEyesMonitor->getRegister()) {
        bigEyesMonitor->close();
        BLogDispatcher::DeReisterMonitor(bigEyesMonitor);
        bigEyesMonitor->setRegister(false);
    }
}


