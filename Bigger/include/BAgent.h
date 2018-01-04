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
    
extern char * leancloudAppID;
extern char * leancloudAppKey;
    
void printLogToConsole(int nType, int bEnable);

void openBigWriter(const char *_logPath, const char * appID, const char * appKey);
void closeBigWriter();

//运行命令执行器
void runCommand(const char *command);

// 写入到日志文件的级别
void setWritterLevel(BLogType level);

// 启用Fatal日志实时上报
void initStatisticsMonitor();
void setUserIdentifier(const char *iden);

// This function is only for Swift
// Don't call this function directly
// For Objective-C, Use Log[DIEF] macro
// For Swift, call log[DIEF] function
void SwiftBiggerLog(BLogType type, const char * log);

//数据持久化
bool persistentWrite(void *pBuf, int nLen);
bool persistentRead(void **pBuf, int *nLen);
bool persistentClear(bool bClear);
    
#if defined(__cplusplus)
}
#endif
        
#endif /* BAgent_h */
