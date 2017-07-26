//
//  main.cpp
//  Bigger
//
//  Created by Edward on 11/7/17.
//  Copyright © 2017年 Netease. All rights reserved.
//

#include <iostream>

#include "BLogger.h"

class ConsolePrinter : public BLogMonitor {
public:
    void Callback(BLogType eLogType, const char *pLog);
};

void ConsolePrinter::Callback(BLogType eLogType, const char *pLog) {
    std::cout<<pLog;
    return;
}


int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, Bigger!\n";
    
    ConsolePrinter printer;
    printer._MonitorType = B_LOG_TYPE_ERROR | B_LOG_TYPE_INFO | B_LOG_TYPE_DEBUG;
    BLogDispatcher::RegisterMonitor(printer);
    
    int x=3;
    LOGE("Have a try--%d\n", x);
    
    LOGI("It works? [%s/%s]\n", "Y", "n");
    
    return 0;
}
