//
//  BigInterpreter.cpp
//  Pods
//
//  Created by 王仕杰 on 2017/8/1.
//
//

#include "BAgent.h"
#include "BiggerFileUploader.h"

void runCommand(const char * identifier) {
    [BiggerFileUploader uploadFileWithPath:@""
                                identifier:[NSString stringWithCString:identifier
                                                              encoding:NSUTF8StringEncoding]
                         completionHandler:^(NSError * _Nullable error) {
                             NSLog(@"Upload file error: %@", error);
                         }];
}
