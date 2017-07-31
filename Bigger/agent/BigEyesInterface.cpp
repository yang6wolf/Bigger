//
//  XlogInterface.cpp
//  Pods
//
//  Created by 杨志超 on 2017/7/27.
//
//

#include "BigEyesInterface.h"
#include "BigEyesMonitor.h"

static BigEyesMonitor *bigEyesMonitor = NULL;

void initBigEyes(const char *_logPath, const char *_filePrefix) {
    if (bigEyesMonitor == NULL)
        bigEyesMonitor = new BigEyesMonitor();
    
    if (bigEyesMonitor->isPathNull())
        bigEyesMonitor->init(_logPath, _filePrefix);
    else {
        printf("BigEyesMonitor has already been inited!\n");
        return;
    }
    
    bigEyesMonitor->open(_logPath, _filePrefix);
    BLogDispatcher::RegisterMonitor(bigEyesMonitor);
    bigEyesMonitor->setRegister(true);
    
}

void openBigEyes(const char *_logPath, const char *_filePrefix) {
    if (bigEyesMonitor == NULL) {
        initBigEyes(_logPath, _filePrefix);
        return;
    }
    if (bigEyesMonitor->getRegister()) {
        printf("BigEyesMonitor has already been opened!\n");
        return;
    }
    bigEyesMonitor->open(_logPath, _filePrefix);
    BLogDispatcher::RegisterMonitor(bigEyesMonitor);
    bigEyesMonitor->setRegister(true);
}

void closeBigEyes() {
    bigEyesMonitor->close();
    BLogDispatcher::DeReisterMonitor(bigEyesMonitor);
    bigEyesMonitor->setRegister(false);
}



