//
//  BiggerFileUploader.h
//  Bigger
//
//  Created by 王仕杰 on 2017/7/28.
//  Copyright © 2017年 NetEase. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface BiggerFileUploader : NSObject

+ (void)uploadFileWithPath:(NSString * _Nonnull)path
                identifier:(NSString * _Nullable)identifier
               isEncrypted:(BOOL)encrypted
         completionHandler:(void (^ _Nullable)(NSError * _Nullable error))completion;

@end
