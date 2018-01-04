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
static int logLevel = B_LOG_TYPE_FATAL | B_LOG_TYPE_ERROR | B_LOG_TYPE_INFO | B_LOG_TYPE_DEBUG;

char * leancloudAppID;
char * leancloudAppKey;

extern void __addListener();
extern void __removeListener();

void openBigWriter(const char *_logPath, const char * appID, const char * appKey) {
    if (bigWriter == NULL) {
        bigWriter = new BigWriter();
    }
    
    bigWriter->setMonitorType(logLevel);
    
    if (bigWriter->isPathNull())
        bigWriter->init(_logPath, true, true);
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
    
    leancloudAppID = (char *)malloc(strlen(appID) + 1);
    strcpy(leancloudAppID, appID);
    
    leancloudAppKey = (char *)malloc(strlen(appKey) + 1);
    strcpy(leancloudAppKey, appKey);
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

void setWritterLevel(BLogType level) {
    logLevel = level;
}
