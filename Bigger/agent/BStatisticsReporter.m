//
//  BStatisticsReporter.m
//  Bigger
//
//  Created by 王仕杰 on 2017/8/3.
//

#import <Foundation/Foundation.h>
#import "BStatisticsReporter.h"

static NSString * const LeanCloudID = @"LVQdV4HdaL5WiQAbycKuMhot-gzGzoHsz";
static NSString * const LeanCloudIDHeaderField = @"X-LC-Id";

static NSString * const LeanCloudKey = @"oQGg6Y8FNQaqAfDdzzHGY6PA";
static NSString * const LeanCloudKeyHeaderField = @"X-LC-Key";

@interface BStatisticsReporter : NSObject

+ (void)reportStatisticsMessage:(NSString *)msg identifier:(NSString *)identifier;

@end


@implementation BStatisticsReporter

+ (void)reportStatisticsMessage:(NSString *)msg identifier:(NSString *)identifier {
    NSDictionary* info = [NSBundle mainBundle].infoDictionary;
    NSDateFormatter* formatter = [NSDateFormatter new];
    formatter.dateFormat = @"yyyy-MM-dd HH:mm:ss.SSS";
    
    NSDictionary* params = @{
                             @"desc" : msg,
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
    
    NSMutableURLRequest* request =
    [NSMutableURLRequest requestWithURL:[NSURL URLWithString:@"https://api.leancloud.cn/1.1/classes/BiggerErrorLog"]];
    request.HTTPMethod = @"POST";
    [request setValue:LeanCloudKey forHTTPHeaderField:LeanCloudKeyHeaderField];
    [request setValue:LeanCloudID forHTTPHeaderField:LeanCloudIDHeaderField];
    [request setValue:@"application/json; charset=utf-8" forHTTPHeaderField:@"Content-Type"];
    
    request.HTTPBody = uploadData;
    
    [[[NSURLSession sharedSession] dataTaskWithRequest:request] resume];
}

@end

void reportStatisticsMessage(const char * msg, const char * identifier) {
    [BStatisticsReporter reportStatisticsMessage:[NSString stringWithCString:msg
                                                                    encoding:NSUTF8StringEncoding]
                                      identifier:[NSString stringWithCString:identifier
                                                                    encoding:NSUTF8StringEncoding]];
}
