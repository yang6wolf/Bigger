//
//  BFileWriterInterface.cc
//  Bigger
//
//  Created by 杨志超 on 2017/7/27.
//
//

#include "BAgentInternal.h"
#include "BFileWriter.h"

static BigWriter *bigWriter = NULL;

extern void __addListener();
extern void __removeListener();

bool bigger_start_write_log(int nType, const char *pFilePath) {
    if (strAppID.empty() || strDeviceID.empty()) {
        printf("bigger_start_write_log error!\n");
        return false;
    }
    
    if (bigWriter == NULL) {
        bigWriter = new BigWriter();
    }
    
    bigWriter->setMonitorType(nType);
    
    if (bigWriter->isPathNull())
        bigWriter->init(pFilePath, true, true);
    else {
        printf("BigWriter has already been opened!\n");
        return false;
    }
    
    if (!bigWriter->getRegister()) {
        bigWriter->open();
        BLogDispatcher::RegisterMonitor(bigWriter);
        bigWriter->setRegister(true);
        __addListener();
    }
    return true;
}

bool checkInit() {
    if (bigWriter == NULL || bigWriter->isPathNull()) {
        printf("BigWriter didn't open!\n");
        return false;
    }
    return true;
}

void bigger_end_write_log() {
    if (!checkInit() || !bigWriter->getRegister())
        return;
    
    bigWriter->close();
    BLogDispatcher::DeReisterMonitor(bigWriter);
    bigWriter->setRegister(false);
    __removeListener();
}
