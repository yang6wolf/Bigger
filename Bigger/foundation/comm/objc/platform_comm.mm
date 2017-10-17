/**
 * created on : 2012-11-28
 * author : yerungui
 */
#include "comm/platform_comm.h"

#include "comm/xlogger/xlogger.h"
#include "comm/xlogger/loginfo_extract.h"

#import <TargetConditionals.h>

#import <UIKit/UIApplication.h>

#include <MacTypes.h>

#if !TARGET_OS_IPHONE
static float __GetSystemVersion() {
    //	float system_version = [UIDevice currentDevice].systemVersion.floatValue;
    //	return system_version;
    NSString *versionString;
    NSDictionary * sv = [NSDictionary dictionaryWithContentsOfFile:@"/System/Library/CoreServices/SystemVersion.plist"];
    if (nil != sv){
        versionString = [sv objectForKey:@"ProductVersion"];
        return versionString != nil ? versionString.floatValue : 0;
    }
    
    return 0;
}
#endif

float publiccomponent_GetSystemVersion() {
    //	float system_version = [UIDevice currentDevice].systemVersion.floatValue;
    //	return system_version;
    NSString *versionString;
    NSDictionary * sv = [NSDictionary dictionaryWithContentsOfFile:@"/System/Library/CoreServices/SystemVersion.plist"];
    if (nil != sv){
        versionString = [sv objectForKey:@"ProductVersion"];
        return versionString != nil ? versionString.floatValue : 0;
    }
    
    return 0;
}

void ConsoleLog(const XLoggerInfo* _info, const char* _log)
{
    //SCOPE_POOL();

    if (NULL==_info || NULL==_log) return;
    
    static const char* levelStrings[] = {
        "V",
        "D",  // debug
        "I",  // info
        "W",  // warn
        "E",  // error
        "F"  // fatal
    };
    
    char strFuncName[128]  = {0};
    ExtractFunctionName(_info->func_name, strFuncName, sizeof(strFuncName));
    
    const char* file_name = ExtractFileName(_info->filename);
    
    char log[16 * 1024] = {0};
    snprintf(log, sizeof(log), "[%s][%s][%s, %s, %d][%s", levelStrings[_info->level], NULL == _info->tag ? "" : _info->tag, file_name, strFuncName, _info->line, _log);
    
    
    NSLog(@"%@", [NSString stringWithUTF8String:log]);
}

void comm_export_symbols_1(){}

