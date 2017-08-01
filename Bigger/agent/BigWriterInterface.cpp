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

void initBigWriter(const char *_logPath, const char *_prefix, bool _isCompress, bool _isCrypt) {
    if (bigWriter == NULL) {
        bigWriter = new BigWriter();
        bigWriter->setMonitorType(B_LOG_TYPE_ERROR | B_LOG_TYPE_INFO | B_LOG_TYPE_DEBUG);
    }
    
    if (bigWriter->isPathNull())
        bigWriter->init(_logPath, _prefix, _isCompress, _isCrypt);
    else {
        printf("BigWriter has already been inited!\n");
        return;
    }
    
    openBigWriter();
}

void initBigWriterWithType(const char *_logPath, const char *_prefix, bool _isCompress, bool _isCrypt, int nType) {
    if (bigWriter == NULL) {
        bigWriter = new BigWriter();
        bigWriter->setMonitorType(nType);
    }
    
    if (bigWriter->isPathNull())
        bigWriter->init(_logPath, _prefix, _isCompress, _isCrypt);
    else {
        printf("BigWriter has already been inited!\n");
        return;
    }
    
    openBigWriter();
}

bool checkInit() {
    if (bigWriter == NULL || bigWriter->isPathNull()) {
        printf("BigWriter didn't init!\n");
        return false;
    }
    return true;
}

void openBigWriterWithType(int nType) {
    if (!checkInit())
        return;
    bigWriter->setMonitorType(nType);
    openBigWriter();
}

void openBigWriter() {
    if (!checkInit() || bigWriter->getRegister())
        return;
    
    bigWriter->open();
    BLogDispatcher::RegisterMonitor(bigWriter);
    bigWriter->setRegister(true);
}

void closeBigWriter() {
    if (!checkInit() || !bigWriter->getRegister())
        return;
    
    bigWriter->close();
    BLogDispatcher::DeReisterMonitor(bigWriter);
    bigWriter->setRegister(false);
}

void flushBigWriter() {
    if (!checkInit() || !bigWriter->getRegister())
        return;
    
    bigWriter->flush();
}

void syncFlushBigWriter() {
    if (!checkInit() || !bigWriter->getRegister())
        return;
    
    bigWriter->syncFlush();
}

