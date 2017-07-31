//
//  BLoggerWrapper.h
//  Bigger
//
//  Created by Edward on 26/7/17.
//  Copyright © 2017年 Netease. All rights reserved.
//

#ifndef BLoggerWrapper_h
#define BLoggerWrapper_h

#include "BCommonDef.h"

#if defined(__cplusplus)
extern "C" {
#endif


void WriteLog(BLogType eLogType, const char *pFormat, ...);


#define LOGD(fmt, ...) WriteLog(B_LOG_TYPE_DEBUG, fmt, ##__VA_ARGS__)
#define LOGI(fmt, ...) WriteLog(B_LOG_TYPE_INFO, fmt, ##__VA_ARGS__)
#define LOGE(fmt, ...) WriteLog(B_LOG_TYPE_ERROR, fmt, ##__VA_ARGS__)
    

#define ILOGD(s, fmt, ...) \
    if(s) {\
        WriteLog(B_LOG_TYPE_DEBUG, fmt, ##__VA_ARGS__);\
    }

#define ILOGI(s, fmt, ...) \
    if(s) {\
        WriteLog(B_LOG_TYPE_INFO, fmt, ##__VA_ARGS__);\
    }

#define ILOGE(s, fmt, ...) \
    if(s) {\
        WriteLog(B_LOG_TYPE_ERROR, fmt, ##__VA_ARGS__);\
    }

    
#if defined(__cplusplus)
}
#endif

#endif /* BLoggerWrapper_h */
