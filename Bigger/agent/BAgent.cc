//
//  BAgent.cc
//  Pods
//
//  Created by Edward on 28/7/17.
//
//

#include "BAgent.h"
#include "BLogger.h"

#include <iostream>

bool bigger_init_sdk(const char *pAppID, const char *pDeviceID) {
    return false;
}

void bigger_release_sdk() {
    
}
/**********Printer**********/
class ConsolePrinter : public BLogMonitor {
public:
    void Callback(BLogType eLogType, const char *pLog);
};

void ConsolePrinter::Callback(BLogType eLogType, const char *pLog) {
    std::cout<<pLog<<"\n";
    return;
}

static ConsolePrinter *pLogPrinter = NULL;
void bigger_print_to_console(int nType, int bEnable) {
    if (bEnable) {
        if (pLogPrinter == NULL) {
            pLogPrinter = new ConsolePrinter();
            BLogDispatcher::RegisterMonitor(pLogPrinter);
        }
        
        pLogPrinter->_MonitorType = nType;
    }
    else {
        BLogDispatcher::DeReisterMonitor(pLogPrinter);
    }
}
/**********Printer**********/

/**********Persistencer**********/
#include <mutex>

#include "boost/iostreams/device/mapped_file.hpp"
#include "fileWriter/src/log_buffer.h"
#include "comm/mmap_util.h"

static std::mutex sg_mutex_access;
static boost::iostreams::mapped_file sg_mmmap_file;
static LogBuffer *sg_log_buff = NULL;
static const unsigned int kBufferBlockLength = 150 * 1024;
static PtrBuffer *sg_reading_buff = NULL;



bool persistentWrite(void *pBuf, int nLen) {
    
    if (sg_reading_buff) {
        printf("Can not write buff while reading");
        return false;
    }
    
    sg_mutex_access.lock();
    if (!sg_mmmap_file.is_open()) {
        std::string strHome = getenv("HOME");
        std::string strFilePath = strHome + "/Documents/Persistencer.mmap2";
        
        if (OpenMmapFile(strFilePath.c_str(), kBufferBlockLength, sg_mmmap_file)) {
           sg_log_buff = new LogBuffer(sg_mmmap_file.data(), kBufferBlockLength, false, false, NULL);
        }
        else {
            sg_mutex_access.unlock();
            return false;
        }
    }

    if (!sg_log_buff) {
        sg_mutex_access.unlock();
        return false;
    }
    
    bool bSuccess = sg_log_buff->Write(pBuf, nLen);
    sg_mutex_access.unlock();
    return bSuccess;
}

bool persistentRead(void **pBuf, int *nLen) {
    if (sg_log_buff == NULL ||
        pBuf == NULL ||
        nLen == NULL) {
        return false;
    }
    
    sg_mutex_access.lock();
    sg_reading_buff = &sg_log_buff->GetData();
    *pBuf = sg_reading_buff->Ptr();
    *nLen = (int)sg_reading_buff->Length();
    sg_mutex_access.unlock();
    
    return true;
}

bool persistentClear(bool bClear) {
    if (sg_log_buff == NULL) {
        return false;
    }
    
    sg_reading_buff = NULL;
    sg_mutex_access.lock();
    if (bClear) {
        AutoBuffer tempBuf;
        sg_log_buff->Flush(tempBuf);
    }
    sg_mutex_access.unlock();
    return true;
}

/**********Persistencer**********/
