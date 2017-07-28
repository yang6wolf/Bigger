//
//  XlogInterface.cpp
//  Pods
//
//  Created by 杨志超 on 2017/7/27.
//
//

#include "BigWriterInterface.h"
#include "BigWriterMonitor.h"

static BigWriterMonitor *bigWriterMonitor = NULL;

void initBigWriter(const char *_logPath, const char *_filePrefix) {
    if (bigWriterMonitor == NULL)
        bigWriterMonitor = new BigWriterMonitor();
    
    if (bigWriterMonitor->isPathNull())
        bigWriterMonitor->init(_logPath, _filePrefix);
    else {
        printf("BigWriterMonitor already is inited!\n");
        return;
    }
    
    bigWriterMonitor->open(_logPath, _filePrefix);
    BLogDispatcher::RegisterMonitor(*bigWriterMonitor);
    bigWriterMonitor->setRegister(true);
    
}

void openBigWriter(const char *_logPath, const char *_filePrefix) {
    if (bigWriterMonitor == NULL) {
        initBigWriter(_logPath, _filePrefix);
        return;
    }
    if (bigWriterMonitor->getRegister()) {
        printf("BigWriterMonitor already is opened!\n");
        return;
    }
    bigWriterMonitor->open(_logPath, _filePrefix);
    BLogDispatcher::RegisterMonitor(*bigWriterMonitor);
    bigWriterMonitor->setRegister(true);
}

void closeBigWriter() {
    bigWriterMonitor->close();
    BLogDispatcher::DeReisterMonitor(*bigWriterMonitor);
    bigWriterMonitor->setRegister(false);
}



