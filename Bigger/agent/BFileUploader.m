//
//  BiggerFileUploader.m
//  Bigger
//
//  Created by 王仕杰 on 2017/7/28.
//  Copyright © 2017年 NetEase. All rights reserved.
//

#import "BFileUploader.h"

static NSString * const LeanCloudID = @"LVQdV4HdaL5WiQAbycKuMhot-gzGzoHsz";
static NSString * const LeanCloudIDHeaderField = @"X-LC-Id";

static NSString * const LeanCloudKey = @"oQGg6Y8FNQaqAfDdzzHGY6PA";
static NSString * const LeanCloudKeyHeaderField = @"X-LC-Key";

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
                                         }
                                     }] resume];
}

+ (NSURLRequest *)requestWithURL:(NSString *)url contentType:(NSString *)type uploadData:(NSData *)data {
    NSMutableURLRequest* request = [NSMutableURLRequest requestWithURL:[NSURL URLWithString:url]];
    
    request.HTTPMethod = @"POST";
    [request setValue:LeanCloudKey forHTTPHeaderField:LeanCloudKeyHeaderField];
    [request setValue:LeanCloudID forHTTPHeaderField:LeanCloudIDHeaderField];
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
    NSDictionary *dict = @{@"name" : name, @"version" : version, @"systemInfo" : systemString, @"logFile" : @{@"id" : logId, @"__type" : @"File"}};
    NSData* bindingData = [NSJSONSerialization dataWithJSONObject:dict
                                                          options:0
                                                            error:nil];
    NSURLRequest* request = [self requestWithURL:path
                                     contentType:@"application/json; charset=utf-8"
                                      uploadData:bindingData];
    
    NSURLSessionDataTask *task = [[NSURLSession sharedSession] dataTaskWithRequest:request completionHandler:^(NSData * _Nullable data, NSURLResponse * _Nullable response, NSError * _Nullable error) {
        
    }];
    [task resume];
}

@end
