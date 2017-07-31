//
//  XlogInterface.cpp
//  Pods
//
//  Created by 杨志超 on 2017/7/27.
//
//

#include "BAgent.h"
#include "BigWriter.h"

static BigWriter *bigWriter = NULL;

void initBigWriter(const char *_logPath, const char *_prefix, bool _isCrypt) {
    if (bigWriter == NULL) {
        bigWriter = new BigWriter();
        bigWriter->setMonitorType(B_LOG_TYPE_ERROR | B_LOG_TYPE_INFO | B_LOG_TYPE_DEBUG);
    }
    
    if (bigWriter->isPathNull())
        bigWriter->init(_logPath, _prefix);
    else {
        printf("BigWriter has already been inited!\n");
        return;
    }
    
    openBigWriter(_isCrypt);
}

void initBigWriterWithType(const char *_logPath, const char *_prefix, bool _isCrypt, int nType) {
    if (bigWriter == NULL) {
        bigWriter = new BigWriter();
        bigWriter->setMonitorType(nType);
    }
    
    if (bigWriter->isPathNull())
        bigWriter->init(_logPath, _prefix);
    else {
        printf("BigWriter has already been inited!\n");
        return;
    }
    
    openBigWriter(_isCrypt);
}

bool checkInit() {
    if (bigWriter == NULL || bigWriter->isPathNull()) {
        printf("BigWriter didn't init!");
        return false;
    }
    return true;
}

void openBigWriterWithType(bool _isCrypt, int nType) {
    if (!checkInit())
        return;
    bigWriter->setMonitorType(nType);
    openBigWriter(_isCrypt);
}

void openBigWriter(bool _isCrypt) {
    if (!checkInit())
        return;
    if (!bigWriter->getRegister()) {
        bigWriter->open();
        BLogDispatcher::RegisterMonitor(bigWriter);
        bigWriter->setRegister(true);
    }
}

void closeBigWriter() {
    if (!checkInit())
        return;
    if (bigWriter->getRegister()) {
        bigWriter->close();
        BLogDispatcher::DeReisterMonitor(bigWriter);
        bigWriter->setRegister(false);
    }
}


