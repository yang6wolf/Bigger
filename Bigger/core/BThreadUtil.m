//
//  BThreadUtil.m
//  Pods
//
//  Created by Edward on 1/8/17.
//
//

#import <Foundation/Foundation.h>

#import "BThreadUtil.h"

@implementation NSThread (GetSequenceNumber)

- (NSInteger)sequenceNumber
{
    return [[self valueForKeyPath:@"private.seqNum"] integerValue];
}

@end


int64_t getCurrnetThreadID() {
    return [[NSThread currentThread] sequenceNumber];
}

int64_t getMainThreadID() {
    return [[NSThread mainThread] sequenceNumber];
}
