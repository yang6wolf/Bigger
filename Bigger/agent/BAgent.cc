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

/**********Printer**********/
class ConsolePrinter : public BLogMonitor {
public:
    void Callback(BLogType eLogType, const char *pLog);
};

void ConsolePrinter::Callback(BLogType eLogType, const char *pLog) {
    std::cout<<pLog<<"\n";
    return;
}

static ConsolePrinter *pLogPrinter=NULL;
void printLogToConsole(int nType, int bEnable) {
    if (bEnable) {
        if (pLogPrinter==NULL) {
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
