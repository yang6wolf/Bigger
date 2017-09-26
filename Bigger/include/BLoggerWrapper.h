//
//  BLoggerWrapper.h
//  Bigger
//
//  Created by Edward on 26/7/17.
//  Copyright © 2017年 NetEase. All rights reserved.
//

#ifndef BLoggerWrapper_h
#define BLoggerWrapper_h

#include "BCommonDef.h"

#if defined(__cplusplus)
extern "C" {
#endif

//default implementation
#ifndef MODULE
#define MODULE "undefine"
#endif
    

void WriteLogF(BLogType eLogType, const char *pModuleName, const char *pFileName, int nLineNumber, const char *pFuncName, const char *pFormat, ...);
void WriteLog(BLogType eLogType, const char *pModuleName, const char *pFileName, int nLineNumber, const char *pFuncName, const char *pBody);
    
#define LOGD(fmt, ...) WriteLogF(B_LOG_TYPE_DEBUG, MODULE, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
#define LOGI(fmt, ...) WriteLogF(B_LOG_TYPE_INFO,  MODULE, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
#define LOGE(fmt, ...) WriteLogF(B_LOG_TYPE_ERROR, MODULE, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
#define LOGF(fmt, ...) WriteLogF(B_LOG_TYPE_FATAL, MODULE, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
    
    
#define ILOGD(s, fmt, ...) \
    if(s) {\
        LOGD(fmt, ##__VA_ARGS__);\
    }

#define ILOGI(s, fmt, ...) \
    if(s) {\
        LOGI(fmt, ##__VA_ARGS__);\
    }

#define ILOGE(s, fmt, ...) \
    if(s) {\
        LOGE(fmt, ##__VA_ARGS__);\
    }
    
#define ILOGF(s, fmt, ...) \
    if(s) {\
        LOGF(fmt, ##__VA_ARGS__);\
    }
  
    
#define CHECK(s, x) \
    if(s) {\
        x;\
    }
    
    
#ifdef __OBJC__
#import<Foundation/Foundation.h>

#define NSCHAR(fmt, ...) ([[NSString stringWithFormat:fmt, ##__VA_ARGS__, nil] UTF8String])

#define NSLOGD(fmt, ...) WriteLog(B_LOG_TYPE_DEBUG, MODULE, __FILE__, __LINE__, __FUNCTION__, NSCHAR(fmt, ##__VA_ARGS__))
#define NSLOGI(fmt, ...) WriteLog(B_LOG_TYPE_INFO,  MODULE, __FILE__, __LINE__, __FUNCTION__, NSCHAR(fmt, ##__VA_ARGS__))
#define NSLOGE(fmt, ...) WriteLog(B_LOG_TYPE_ERROR, MODULE, __FILE__, __LINE__, __FUNCTION__, NSCHAR(fmt, ##__VA_ARGS__))
#define NSLOGF(fmt, ...) WriteLog(B_LOG_TYPE_FATAL, MODULE, __FILE__, __LINE__, __FUNCTION__, NSCHAR(fmt, ##__VA_ARGS__))

#endif

    
#if defined(__cplusplus)
}
#endif

#endif /* BLoggerWrapper_h */
