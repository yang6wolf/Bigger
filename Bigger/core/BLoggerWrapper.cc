//
//  BLoggerWrapper.cc
//  Bigger
//
//  Created by Edward on 26/7/17.
//  Copyright © 2017年 NetEase. All rights reserved.
//

#include "BLoggerWrapper.h"

#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/time.h>

#include <future>

#include "BLogger.h"
#include "BLogFormatter.h"
#include "BThreadUtil.h"



void WriteLogF(BLogType eLogType, const char *pModuleName, const char *pFileName, int nLineNumber, const char *pFuncName, const char *pFormat, ...) {
    va_list arg;
    va_start(arg, pFormat);
    char pBody[1024]={};
    vsnprintf(pBody, sizeof(pBody)-1, pFormat, arg);
    va_end(arg);
    
    WriteLog(eLogType, pModuleName, pFileName, nLineNumber, pFuncName, pBody);
}

void WriteLog(BLogType eLogType, const char *pModuleName, const char *pFileName, int nLineNumber, const char *pFuncName, const char *pBody) {
    
    timeval tv;
    gettimeofday(&tv, NULL);
    
    char pLog[1024]={};
    formatLogHeader(pLog, eLogType, getpid(), getCurrnetThreadID(), getMainThreadID(), pModuleName, pFileName, nLineNumber, pFuncName, &tv, pBody);
    
    std::async(std::launch::async, BLogDispatcher::WriteLog, eLogType, pLog);
}
