//
//  BStatisticsReporter.m
//  Bigger
//
//  Created by 王仕杰 on 2017/8/3.
//

#import <Foundation/Foundation.h>
#import "BStatisticsReporter.h"
#import "Bigger.h"
#import <sys/utsname.h>

static NSString * const LeanCloudIDHeaderField = @"X-LC-Id";
static NSString * const LeanCloudKeyHeaderField = @"X-LC-Key";

@interface BStatisticsReporter : NSObject

+ (void)reportStatisticsMessage:(NSString *)msg identifier:(NSString *)identifier;

@end


@implementation BStatisticsReporter

+ (void)reportStatisticsMessage:(NSString *)msg identifier:(NSString *)identifier {
    NSDictionary* info = [NSBundle mainBundle].infoDictionary;
    NSDateFormatter* formatter = [NSDateFormatter new];
    formatter.dateFormat = @"yyyy-MM-dd HH:mm:ss.SSS";
    
    NSAssert(msg, @"msg should not be nil");
    
    NSDictionary* params = @{
                             @"desc" : msg ?: @"Message body is nil, please check the line of adding this log.",
                             @"shortDesc" : [msg componentsSeparatedByString:@"] "].lastObject ?: @"",
                             @"identifier" : identifier ?: @"",
                             @"localUpdateTime" : [formatter stringFromDate:[NSDate date]],
                             
                             @"bundleVersion" : info[@"CFBundleShortVersionString"] ?: @"",
                             @"buildVersion" : info[@"CFBundleVersion"] ?: @"",
                             @"displayName" : info[@"CFBundleDisplayName"] ?: @"",
                             
                             @"osVersion" : [UIDevice currentDevice].systemVersion,
                             @"deviceName" : [UIDevice currentDevice].systemName
                             };
    
    // APM
    struct utsname i;
    uname(&i);
    NSString *deviceInfo = [NSString stringWithCString:i.machine encoding:NSUTF8StringEncoding];
    
    NSString* ua = [NSUserDefaults.standardUserDefaults stringForKey:@"UserAgent"];
    __block NSString *EPSessionID, *MASessionID;
    [[ua componentsSeparatedByString:@" "] enumerateObjectsUsingBlock:^(NSString * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
        if ([obj hasPrefix:@"LsessionId"]) {
            EPSessionID = obj.lastPathComponent;
        } else if ([obj hasPrefix:@"LDMASessionId"]) {
            MASessionID = obj.lastPathComponent;
        }
        
        *stop = EPSessionID && MASessionID;
    }];
    
    NSMutableDictionary* extraAPMParams = [@{
                                     @"id" : [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleIdentifier"] ?: @"", // product id
                                     @"m" :  deviceInfo ?: @"", // device info
                                     @"o" : [UIDevice currentDevice].systemVersion ?: @"", // os version
                                     @"sid_ep" : EPSessionID ?: @"",
                                     @"sid" : MASessionID ?: @"",
                                     } mutableCopy];
    [extraAPMParams addEntriesFromDictionary:params];
    
    NSMutableURLRequest* apmRequest = [NSMutableURLRequest requestWithURL:[NSURL URLWithString:@"https://mt.analytics.163.com/fatal_error"]];
    apmRequest.HTTPMethod = @"POST";
    [apmRequest setValue:@"application/json; charset=utf-8" forHTTPHeaderField:@"Content-Type"];
    apmRequest.HTTPBody = [NSJSONSerialization dataWithJSONObject:extraAPMParams
                                                          options:0
                                                            error:nil];
    [[[NSURLSession sharedSession] dataTaskWithRequest:apmRequest] resume];
    
    NSData* uploadData = [NSJSONSerialization dataWithJSONObject:params
                                                         options:0
                                                           error:nil];
    // Leancloud
    assert(leancloudAppKey && leancloudAppID);
    if (!leancloudAppKey || !leancloudAppID) {
        return;
    }

    NSMutableURLRequest* leanCloudRequest = [NSMutableURLRequest requestWithURL:[NSURL URLWithString:@"https://api.leancloud.cn/1.1/classes/BiggerErrorLog"]];
    
    [leanCloudRequest setValue:[NSString stringWithCString:leancloudAppKey encoding:NSUTF8StringEncoding]
   forHTTPHeaderField:LeanCloudKeyHeaderField];
    [leanCloudRequest setValue:[NSString stringWithCString:leancloudAppID encoding:NSUTF8StringEncoding]
   forHTTPHeaderField:LeanCloudIDHeaderField];
    leanCloudRequest.HTTPMethod = @"POST";
    [leanCloudRequest setValue:@"application/json; charset=utf-8" forHTTPHeaderField:@"Content-Type"];
    leanCloudRequest.HTTPBody = uploadData;
    
    [[[NSURLSession sharedSession] dataTaskWithRequest:leanCloudRequest] resume];

    // ZWW
    NSMutableURLRequest *reqLogstash = [NSMutableURLRequest requestWithURL:[NSURL URLWithString:@"https://zwwdata.ms.netease.com:8080"]];
    reqLogstash.HTTPMethod = @"POST";
    [reqLogstash setValue:@"application/json; charset=utf-8" forHTTPHeaderField:@"Content-Type"];
//    NSString *strBody = @"[RoomID:999][TimeStamp:18-02-02 11:59:31][MSG:LiveStream num 1 start ok]";
//    reqLogstash.HTTPBody = [strBody dataUsingEncoding:NSUTF8StringEncoding];
    reqLogstash.HTTPBody = uploadData;
     [[[NSURLSession sharedSession] dataTaskWithRequest:reqLogstash] resume];
}

@end

void reportStatisticsMessage(const char * msg, const char * identifier) {
    NSString* message = [NSString stringWithCString:msg
                                           encoding:NSUTF8StringEncoding];
    
    if (!message) {
        long length = strlen(msg);
        char* m = malloc(length);
        strcpy(m, msg);
        for (long l = length - 2; l > length - 8; l--) {
            m[l] = '\0';
            message = [NSString stringWithCString:m encoding:NSUTF8StringEncoding];
            if (message) {
                break;
            }
        }
        free(m);
    }
    
    [BStatisticsReporter reportStatisticsMessage:message
                                      identifier:[NSString stringWithCString:identifier
                                                                    encoding:NSUTF8StringEncoding]];
    
}
