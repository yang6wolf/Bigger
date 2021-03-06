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
    dispatch_queue_t uploadQueue = dispatch_queue_create("Bigger.UploadQueue", DISPATCH_QUEUE_SERIAL);
    
    NSString* iden = !identifier ? nil : [NSString stringWithCString:identifier
                                                            encoding:NSUTF8StringEncoding];
    assert(iden.length > 0);
    
    dispatch_async(uploadQueue, ^{
        BigWriter instance;
        
        instance.syncFlush();
        
        NSString* path = [[NSString stringWithCString:instance.getPath()
                                             encoding:NSUTF8StringEncoding] stringByAppendingPathComponent:@"dailylog.plog"];
        
        [BiggerFileUploader uploadFileWithPath:path
                                    identifier:iden
                                   isEncrypted:instance.getCrypt()
                             completionHandler:^(NSError * _Nullable error) {
                                 NSLog(@"Upload file error: %@", error);
                             }];
    });
}


void runCommand(const char * command) {
    if (1) {
        uploadLog(command);
    }
}
