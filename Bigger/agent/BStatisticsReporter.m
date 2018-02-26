//
//  BStatisticsReporter.m
//  Bigger
//
//  Created by 王仕杰 on 2017/8/3.
//

#import <Foundation/Foundation.h>
#import "BStatisticsReporter.h"
#import "Bigger.h"

#import "../thirdparty/curl/include/curl.h"

static NSString * const LeanCloudIDHeaderField = @"X-LC-Id";
static NSString * const LeanCloudKeyHeaderField = @"X-LC-Key";

char * leancloudAppID;
char * leancloudAppKey;

@interface BStatisticsReporter : NSObject

+ (void)reportStatisticsMessage:(NSString *)msg identifier:(NSString *)identifier;

@end


@implementation BStatisticsReporter

+ (void)reportStatisticsMessage:(NSString *)msg identifier:(NSString *)identifier {
    //报数据给统计中心
    NSMutableURLRequest *reqLogstash = [NSMutableURLRequest requestWithURL:[NSURL URLWithString:@"http://zwwdata.ms.netease.com:8080"]];
    reqLogstash.HTTPMethod = @"POST";
    [reqLogstash setValue:@"application/json; charset=utf-8" forHTTPHeaderField:@"Content-Type"];
//    NSString *strBody = @"[RoomID:999][TimeStamp:18-02-02 11:59:31][MSG:Test 测试数据]";
//    reqLogstash.HTTPBody = [strBody dataUsingEncoding:NSUTF8StringEncoding];
    reqLogstash.HTTPBody = [msg dataUsingEncoding:NSUTF8StringEncoding];
    [[[NSURLSession sharedSession] dataTaskWithRequest:reqLogstash] resume];
    
    curl_global_init(CURL_GLOBAL_ALL);
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if(curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL,
                         "http://www.baidu.com");
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    
    //报数据至LeanCloud(后续优化)
//    assert(leancloudAppKey && leancloudAppID);
    if (!leancloudAppKey || !leancloudAppID) {
        return;
    }
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
    NSData* uploadData = [NSJSONSerialization dataWithJSONObject:params
                                                         options:0
                                                           error:nil];
    
    NSMutableURLRequest* apmRequest = [NSMutableURLRequest requestWithURL:[NSURL URLWithString:@"https://mt.analytics.163.com/fatal_error"]];
    apmRequest.HTTPMethod = @"POST";
    [apmRequest setValue:@"application/json; charset=utf-8" forHTTPHeaderField:@"Content-Type"];
    apmRequest.HTTPBody = uploadData;
    [[[NSURLSession sharedSession] dataTaskWithRequest:apmRequest] resume];

    NSMutableURLRequest* leanCloudRequest = [NSMutableURLRequest requestWithURL:[NSURL URLWithString:@"https://api.leancloud.cn/1.1/classes/BiggerErrorLog"]];
    
    [leanCloudRequest setValue:[NSString stringWithCString:leancloudAppKey encoding:NSUTF8StringEncoding]
   forHTTPHeaderField:LeanCloudKeyHeaderField];
    [leanCloudRequest setValue:[NSString stringWithCString:leancloudAppID encoding:NSUTF8StringEncoding]
   forHTTPHeaderField:LeanCloudIDHeaderField];
    leanCloudRequest.HTTPMethod = @"POST";
    [leanCloudRequest setValue:@"application/json; charset=utf-8" forHTTPHeaderField:@"Content-Type"];
    leanCloudRequest.HTTPBody = uploadData;
    [[[NSURLSession sharedSession] dataTaskWithRequest:leanCloudRequest] resume];
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
