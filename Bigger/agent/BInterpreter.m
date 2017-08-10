//
//  BigInterpreter.cpp
//  Pods
//
//  Created by 王仕杰 on 2017/8/1.
//
//

#include "BAgent.h"
#include "BFileUploader.h"

void uploadLog(const char * identifier) {
    NSString* fixedPath = [[NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0] stringByAppendingString:@"/log/dailylog.plog"];
    
    [BiggerFileUploader uploadFileWithPath:fixedPath
                                identifier:[NSString stringWithCString:identifier
                                                              encoding:NSUTF8StringEncoding]
                               isEncrypted:YES
                         completionHandler:^(NSError * _Nullable error) {
                             NSLog(@"Upload file error: %@", error);
                         }];
}


void runCommand(const char * command) {
    if (1) {
        uploadLog(command);
    }
}
