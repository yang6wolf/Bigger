//
//  BAgent.h
//  Pods
//
//  Created by Edward on 28/7/17.
//
//

#ifndef BAgent_h
#define BAgent_h

#import "BCommonDef.h"

#if defined(__cplusplus)
extern "C" {
#endif
    
void printLogToConsole(int nType, int bEnable);

void openBigWriter(const char *_logPath);
void closeBigWriter();
    
void runCommand(const char * command);

// 启用Fatal日志实时上报
void initStatisticsMonitor();
void setUserIdentifier(const char * iden);

// This function is only for Swift
// Don't call this function directly
// For Objective-C, Use Log[DIEF] macro
// For Swift, call log[DIEF] function
void SwiftBiggerLog(BLogType type, const char * log);

#if defined(__cplusplus)
}
#endif
        
#endif /* BAgent_h */
