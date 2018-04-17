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

#import "curl/curl.h"

static NSString * const LeanCloudIDHeaderField = @"X-LC-Id";
static NSString * const LeanCloudKeyHeaderField = @"X-LC-Key";

char *leancloudAppID  = "Qbyt6dGsKpH9q6dP6jpcdmL3-gzGzoHsz";
char *leancloudAppKey = "Nl9yA0zKgUjGts2sYdlVlX5A";

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
    
/*
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if(curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL,
                         "https://www.baidu.com");
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_DIGEST | CURLAUTH_BASIC | CURLAUTH_NEGOTIATE);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
*/
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
