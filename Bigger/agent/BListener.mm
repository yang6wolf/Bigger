//
//  BBackgroundListener.m
//  Pods
//
//  Created by 杨志超 on 2017/8/3.
//
//

#import "BListener.h"

#include "BFileWriter.h"

@implementation BListener

+ (BListener *)sharedInstanced {
    static BListener *_instance = nil;
    static dispatch_once_t onceToken;
    
    dispatch_once(&onceToken, ^{
        _instance = [[self alloc] init];
    });
    
    return _instance;
}

- (void)addBackgroundListener {
    [[NSNotificationCenter defaultCenter] addObserver:[[self class] sharedInstanced] selector:@selector(flushForDidEnterBackground:)
                                                 name:UIApplicationDidEnterBackgroundNotification object:nil];
}

- (void)addTerminateListener {
    [[NSNotificationCenter defaultCenter] addObserver:[[self class] sharedInstanced] selector:@selector(flushForWillTerminate:)
                                                 name:UIApplicationWillTerminateNotification object:nil];
}
    
- (void)removeBackgroundListener {
    [[NSNotificationCenter defaultCenter] removeObserver:[[self class] sharedInstanced] name:UIApplicationDidEnterBackgroundNotification object:nil];
}
    
- (void)removeTerminateListener {
    [[NSNotificationCenter defaultCenter] removeObserver:[[self class] sharedInstanced] name:UIApplicationWillTerminateNotification object:nil];
}

- (void)flushForDidEnterBackground:(NSNotification *)notification
{
    BigWriter tempWriter;
    tempWriter.flush();
}
    
- (void)flushForWillTerminate:(NSNotification *)notification
{
    BigWriter tempWriter;
    tempWriter.flush();
}
@end

void __addListener() {
    [[BListener sharedInstanced] addBackgroundListener];
    [[BListener sharedInstanced] addTerminateListener];
}

void __removeListener() {
    [[BListener sharedInstanced] removeBackgroundListener];
    [[BListener sharedInstanced] removeTerminateListener];
}
