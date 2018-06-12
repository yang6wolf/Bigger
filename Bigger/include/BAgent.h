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
#include "stdbool.h"

#if defined(__cplusplus)
extern "C" {
#endif
    
    // 1.初始化SDK接口(仅用于格式化日志,尚未做权限校验)
    bool bigger_init_sdk(const char *pAppID, const char *pDeviceID);
    void bigger_release_sdk(void);
    
    // 2.开启控制台打印
    void bigger_print_to_console(int nType, int bEnable);
    
    // 3.开启日志实时上报
    typedef char* (*LogFormatter)(const char * log);
    bool bigger_start_realtime_report(int nType, const char *pURL, int h_size, const char * headers[], LogFormatter formatter);
    void bigger_end_realtime_report(const char *pURL);
    
    // 4.生成日志文件
    bool bigger_start_write_log(int nType, const char *pFilePath, const char * filename);
    void bigger_flush_all_logs(void);
    void bigger_end_write_log(const char* pFilePath);
    
    // 5.运行命令执行器
    // Command: APM -u"https://url.to.upload"
    //              -p"/path/to/file"
    //              -hContent-Type: application/json
    //              -a"DeviceID=something"
    
    // Command: LC -u"https://url.to.upload"
    //             -b https://url.to.bind
    //             -p"/path/to/file"
    //             -hContent-Type: application/json
    //             -a"DeviceID=something"
    
    // This is a SYNC function with network usage.
    // Callers should call this function asynchronously by your self
    bool bigger_run_command(const char *pCommand);
    
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
