//
//  BLogFormatter.h
//  Pods
//
//  Created by Edward on 31/7/17.
//
//

#ifndef BLogFormatter_h
#define BLogFormatter_h

#include <stdio.h>
#include <inttypes.h>

#include "BCommonDef.h"

void formatLogHeader(char *pBuf, BLogType eLogType, intmax_t nPID, intmax_t nTID, intmax_t nMainTID, const char *pFileName, int nLineNumber, const char *pFuncName, struct timeval *pTimeval, const char *pBody);

#endif /* BLogFormatter_h */
