//
//  BiggerFileUploader.m
//  Bigger
//
//  Created by 王仕杰 on 2017/7/28.
//  Copyright © 2017年 NetEase. All rights reserved.
//

#import "BFileUploader.h"
#import "Bigger.h"

#import <sys/utsname.h>

#define APMFileEnable 0

static NSString * const LeanCloudIDHeaderField = @"X-LC-Id";
static NSString * const LeanCloudKeyHeaderField = @"X-LC-Key";

extern char * leancloudAppID;
extern char * leancloudAppKey;

@implementation BiggerFileUploader

+ (void)uploadFileWithPath:(NSString *)path
                identifier:(NSString *)identifier
               isEncrypted:(BOOL)encrypted
         completionHandler:(void (^)(NSError * _Nullable error))completion {
    // loading data from path
    NSError* loadingDataError;
    
    NSData* uploadData = [NSData dataWithContentsOfFile:path
                                                options:0
                                                  error:&loadingDataError];
    
    if (loadingDataError) {
        if (completion) {
            completion(loadingDataError);
        }
        NSLOGF(@"Loading data error. Path:(%@), errorDescription:(%@)", path, loadingDataError.localizedDescription);
        return;
    }
    
    // date -> String
    NSDateFormatter* formatter = [NSDateFormatter new];
    formatter.dateFormat = @"yyyy-MM-dd-HH-mm-ss";
    
    NSString* dateString = [formatter stringFromDate:[NSDate date]];
    
    NSURLRequest* request;
    NSString* contentType = @"text/plain; charset=utf-8";
    NSString* nameExtension = @".log";
    
    if (encrypted) {
        contentType = @"application/octet-stream";
        nameExtension = @".plog";
    }
    
    // generate URL request
    if (identifier) {
        request = [self requestWithURL:[NSString stringWithFormat:@"https://api.leancloud.cn/1.1/files/Bigger-%@-%@%@", identifier, dateString, nameExtension]
                           contentType:contentType
                            uploadData:uploadData];
    } else {
        request = [self requestWithURL:[NSString stringWithFormat:@"https://api.leancloud.cn/1.1/files/Bigger-%@%@", dateString, nameExtension]
                           contentType:contentType
                            uploadData:uploadData];
    }
    
    [[[NSURLSession sharedSession] dataTaskWithRequest:request
                                     completionHandler:^(NSData * _Nullable data, NSURLResponse * _Nullable response, NSError * _Nullable error) {
                                         if (completion) {
                                             completion(error);
                                         }
                                         if (!error) {
                                             NSDictionary *json = [NSJSONSerialization JSONObjectWithData:data
                                                                                                  options:NSJSONReadingAllowFragments
                                                                                                    error:nil];
                                             
                                             if (json[@"objectId"]) {
                                                 [self associateNetlogWithObject:json[@"objectId"]
                                                                            name:identifier];
                                             }
                                         } else {
                                             NSLOGF(@"Upload file (leancloud) error. Error:(%@)", error);
                                         }
                                     }] resume];
    
    
#if APMFileEnable
    // loading data from path
    NSError* loadingDataError;

    NSData* uploadData = [NSData dataWithContentsOfFile:path
                                                options:0
                                                  error:&loadingDataError];
    
    if (loadingDataError) {
        if (completion) {
            completion(loadingDataError);
        }
        NSLOGF(@"Loading data error. Path:(%@), errorDescription:(%@)", path, loadingDataError.localizedDescription);
        return;
    }
    
    NSString* boundary = [NSString stringWithFormat:@"--------------------------BiggerUploader%ld", (NSInteger)[NSDate date].timeIntervalSince1970];
    
    NSMutableURLRequest* request = [NSMutableURLRequest requestWithURL:[NSURL URLWithString:@"https://mt.analytics.163.com/upload"]];
    
    request.HTTPMethod = @"POST";
    [request addValue:[NSString stringWithFormat:@"multipart/form-data; boundary=%@", boundary] forHTTPHeaderField:@"content-type"];
    
    NSMutableString* body = [NSMutableString string];
    
    NSString *shortVersion = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleShortVersionString"];
    NSString *buildString = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleVersion"];
    NSString *version = [NSString stringWithFormat:@"%@(%@)", shortVersion, buildString];
    NSString *system = [[UIDevice currentDevice] systemName];
    NSString *systemVersion = [[UIDevice currentDevice] systemVersion];
    NSString *systemString = [NSString stringWithFormat:@"%@,%@", system, systemVersion];
    
    struct utsname info;
    uname(&info);
    NSString *deviceInfo = [NSString stringWithCString:info.machine encoding:NSUTF8StringEncoding];
    
    NSDictionary<NSString *, NSString *> * params = @{@"o" : systemString,
                                                      @"v" : version,
                                                      @"id" : @"id",
                                                      @"u" : @"u",
                                                      @"iden" : identifier,
                                                      @"m" : deviceInfo,
                                                      @"sv" : @"sv",
                                                      };
    
    [params enumerateKeysAndObjectsUsingBlock:^(NSString * _Nonnull key, NSString * _Nonnull obj, BOOL * _Nonnull stop) {
        [body appendFormat:@"--%@", boundary];
        [body appendFormat:@"\nContent-Disposition: form-data; name=\"%@\"\n\n", key];
        [body appendFormat:@"%@\n", obj];
    }];
    
    [body appendFormat:@"--%@", boundary];
    [body appendFormat:@"\nContent-Disposition: form-data; name=\"%@\"; filename=\"123\"", @"file"];
    [body appendString:@"\nContent-Type: application/octet-stream\n\n"];
    NSMutableData* bodyData = [[body dataUsingEncoding:NSUTF8StringEncoding] mutableCopy];
    [bodyData appendData:uploadData];
    [bodyData appendData:[[NSString stringWithFormat:@"\n--%@--", boundary] dataUsingEncoding:NSUTF8StringEncoding]];
    
    request.HTTPBody = bodyData;
    
    [[[NSURLSession sharedSession] dataTaskWithRequest:request
                                     completionHandler:^(NSData * _Nullable data, NSURLResponse * _Nullable response, NSError * _Nullable error) {
                                         if (completion) {
                                             completion(error);
                                         }
                                         if (error) {
                                             NSLOGF(@"Upload file error. Error:(%@)", error);
                                         }
                                     }] resume];
#endif
}

+ (NSURLRequest *)requestWithURL:(NSString *)url contentType:(NSString *)type uploadData:(NSData *)data {
    NSMutableURLRequest* request = [NSMutableURLRequest requestWithURL:[NSURL URLWithString:url]];
    
    request.HTTPMethod = @"POST";
    
    assert(leancloudAppKey && leancloudAppID);
    if (!leancloudAppKey || !leancloudAppID) {
        return nil;
    }
    
    [request setValue:[NSString stringWithCString:leancloudAppKey encoding:NSUTF8StringEncoding]
   forHTTPHeaderField:LeanCloudKeyHeaderField];
    [request setValue:[NSString stringWithCString:leancloudAppID encoding:NSUTF8StringEncoding]
   forHTTPHeaderField:LeanCloudIDHeaderField];
    
    [request setValue:type forHTTPHeaderField:@"Content-Type"];
    
    request.HTTPBody = data;
    
    return [request copy];
}

+ (void)associateNetlogWithObject:(NSString *)logId name:(NSString *)name
{
    NSString *path = [NSString stringWithFormat:@"https://api.leancloud.cn/1.1/classes/Bigger"];
    NSString *shortVersion = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleShortVersionString"];
    NSString *buildString = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleVersion"];
    NSString *version = [NSString stringWithFormat:@"%@(%@)", shortVersion, buildString];
    NSString *system = [[UIDevice currentDevice] systemName];
    NSString *systemVersion = [[UIDevice currentDevice] systemVersion];
    NSString *systemString = [NSString stringWithFormat:@"%@,%@", system, systemVersion];
    NSDictionary *dict = @{@"name" : name ?: @"Undefined", @"version" : version, @"systemInfo" : systemString, @"logFile" : @{@"id" : logId, @"__type" : @"File"}};
    NSData* bindingData = [NSJSONSerialization dataWithJSONObject:dict
                                                          options:0
                                                            error:nil];
    NSURLRequest* request = [self requestWithURL:path
                                     contentType:@"application/json; charset=utf-8"
                                      uploadData:bindingData];
    
    NSURLSessionDataTask *task = [[NSURLSession sharedSession] dataTaskWithRequest:request completionHandler:^(NSData * _Nullable data, NSURLResponse * _Nullable response, NSError * _Nullable error) {
        CHECK(error, NSLOGF(@"Binding class error. Error:(%@)", error));
    }];
    [task resume];
}

@end
