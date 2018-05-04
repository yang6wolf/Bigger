//
//  BFileWriterInterface.cc
//  Bigger
//
//  Created by 杨志超 on 2017/7/27.
//
//

#include "BAgentInternal.h"
#include "BFileWriter.h"
#include "thread.h"
#include <vector>

std::vector<BigWriter *> global_writers;

static void flush_all_writers() {
    while (true) {
        for (auto it = global_writers.cbegin(); it != global_writers.cend(); it++) {
            (*it) -> flush();
        }
        sleep(15 * 60);
    }
}

Thread flush_thread(&flush_all_writers);

bool bigger_start_write_log(int nType, const char *pFilePath, const char * filename) {
    if (!flush_thread.isruning()) {
        flush_thread.start();
    }
    if (strAppID.empty() || strDeviceID.empty()) {
        printf("bigger_start_write_log error!\n");
        return false;
    }
    
    for (auto it = global_writers.cbegin(); it != global_writers.cend(); it++) {
        if (!strcmp((**it).getPath(), pFilePath) && !strcmp((**it).getFilename(), filename)) {
            // same path and file name
            printf("File writer with same path is already exist. To change the type, close and reopen it.\n");
            return false;
        }
    }
    
    auto bigWriter = new BigWriter();
    global_writers.push_back(bigWriter);
    
    bigWriter->setMonitorType(nType);
    
    if (bigWriter->isPathNull())
        bigWriter->init(pFilePath, true, true);
    else {
        printf("BigWriter has already been opened!\n");
        delete bigWriter;
        return false;
    }
    
    if (!bigWriter->getRegister()) {
        bigWriter->open(filename);
        BLogDispatcher::RegisterMonitor(bigWriter);
        bigWriter->setRegister(true);
    }
    return true;
}

void bigger_end_write_log(const char* pFilePath) {    
    for (auto it = global_writers.cbegin(); it != global_writers.cend(); it++) {
        if (!strcmp((**it).getPath(), pFilePath)) {
            // same path
            (*it) -> close();
            BLogDispatcher::DeReisterMonitor((*it));
            it = global_writers.erase(it);
            delete *it;
        }
    }
}
