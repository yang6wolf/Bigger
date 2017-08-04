//
//  XlogInterface.cpp
//  Pods
//
//  Created by 杨志超 on 2017/7/27.
//
//

#include "BAgent.h"
#include "BFileWriter.h"

static BigWriter *bigWriter = NULL;

extern void __addListener();
extern void __removeListener();

void openBigWriter(const char *_logPath, const char *_prefix, bool _isCompress, bool _isCrypt) {
    if (bigWriter == NULL) {
        bigWriter = new BigWriter();
    }
    
#if DEBUG
    bigWriter->setMonitorType(B_LOG_TYPE_FATAL | B_LOG_TYPE_ERROR | B_LOG_TYPE_INFO | B_LOG_TYPE_DEBUG);
#else
    bigWriter->setMonitorType(B_LOG_TYPE_FATAL | B_LOG_TYPE_ERROR | B_LOG_TYPE_INFO);
#endif
    
    if (bigWriter->isPathNull())
        bigWriter->init(_logPath, _prefix, _isCompress, _isCrypt);
    else {
        printf("BigWriter has already been opened!\n");
        return;
    }
    
    if (!bigWriter->getRegister()) {
        bigWriter->open();
        BLogDispatcher::RegisterMonitor(bigWriter);
        bigWriter->setRegister(true);
        __addListener();
    }
}

bool checkInit() {
    if (bigWriter == NULL || bigWriter->isPathNull()) {
        printf("BigWriter didn't open!\n");
        return false;
    }
    return true;
}

void closeBigWriter() {
    if (!checkInit() || !bigWriter->getRegister())
        return;
    
    bigWriter->close();
    BLogDispatcher::DeReisterMonitor(bigWriter);
    bigWriter->setRegister(false);
    __removeListener();
}
