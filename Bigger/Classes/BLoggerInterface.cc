//
//  BLoggerInterface.cc
//  Bigger
//
//  Created by Edward on 26/7/17.
//  Copyright © 2017年 Netease. All rights reserved.
//

#include "BLoggerInterface.h"
#include "BigWriterInterface.h"

#include "BLogger.h"

#include <stdio.h>
#include <stdarg.h>

void WriteLog(BLogType eLogType, const char *pFormat, ...) {
    va_list arg;
    va_start(arg, pFormat);
    char pBuf[1024]={};
    vsnprintf(pBuf, sizeof(pBuf)-1, pFormat, arg);
    BLogDispatcher::WriteLog(eLogType, pBuf);
    va_end(arg);
}
