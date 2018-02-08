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

// 1.初始化SDK接口(仅用于格式化日志,尚未做权限校验)
bool bigger_init_sdk(const char *pAppID, const char *pDeviceID);
void bigger_release_sdk();

// 2.开启控制台打印
void bigger_print_to_console(int nType, int bEnable);

// 3.开启日志实时上报
bool bigger_start_realtime_report(int nType, const char *pURL, int nSize, const char **pArrKeys, const char **pArrVals);
void bigger_end_realtime_report();

// 4.生成日志文件
bool bigger_start_write_log(int nType, const char *pFilePath);
void bigger_end_write_log();
    
// 5.运行命令执行器
bool bigger_run_command(const char *pCommand);
    
    
//To be removed
//void setUserIdentifier(const char *iden);
//void openBigWriter(const char *_logPath, const char * appID, const char * appKey);
//void closeBigWriter();

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
