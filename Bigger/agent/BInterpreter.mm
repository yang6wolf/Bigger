//
//  BigInterpreter.cpp
//  Pods
//
//  Created by 王仕杰 on 2017/8/1.
//
//

#include "BAgent.h"
#include "BFileUploader.h"
#include "BFileWriter.h"

void uploadLog(const char * identifier) {
    NSString* fixedPath = [[NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0] stringByAppendingString:@"/log/dailylog.plog"];
    
    BigWriter instance;
    
    NSString* path = [[NSString stringWithCString:instance.getPath()
                                         encoding:NSUTF8StringEncoding] stringByAppendingPathComponent:@"dailylog.plog"];
    
    [BiggerFileUploader uploadFileWithPath:path
                                identifier:[NSString stringWithCString:identifier
                                                              encoding:NSUTF8StringEncoding]
                               isEncrypted:instance.getCrypt()
                         completionHandler:^(NSError * _Nullable error) {
                             NSLog(@"Upload file error: %@", error);
                         }];
}


void runCommand(const char * command) {
    if (1) {
        uploadLog(command);
    }
}
