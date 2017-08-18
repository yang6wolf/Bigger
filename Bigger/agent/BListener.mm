//
//  BBackgroundListener.m
//  Pods
//
//  Created by 杨志超 on 2017/8/3.
//
//

#import "BListener.h"

#include "BFileWriter.h"

#define FLUSH_TIME 300
@interface BListener()

@property (strong, nonatomic) NSTimer *flushTimer;//定时器
@property (strong, nonatomic) NSThread *timerThread;//执行定时器的子线程
    
@end

@implementation BListener

+ (BListener *)sharedInstanced {
    static BListener *_instance = nil;
    static dispatch_once_t onceToken;
    
    dispatch_once(&onceToken, ^{
        _instance = [[self alloc] init];
    });
    
    return _instance;
}

- (void)initTimerThread {
    @autoreleasepool
    {
        _flushTimer = [NSTimer scheduledTimerWithTimeInterval:FLUSH_TIME target:self selector:@selector(flushForTimer) userInfo:nil repeats:YES];
        [[NSRunLoop currentRunLoop] run];
    }
}
    
- (void)addTimer {
    _timerThread = [[NSThread alloc] initWithTarget:self selector:@selector(initTimerThread) object:nil];
    [_timerThread start];
}

- (void)addBackgroundListener {
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(flushForDidEnterBackground:)
                                                 name:UIApplicationDidEnterBackgroundNotification object:nil];
}

- (void)addTerminateListener {
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(flushForWillTerminate:)
                                                 name:UIApplicationWillTerminateNotification object:nil];
}

- (void)removeTimer {
    //取消定时器
    [_flushTimer invalidate];
    _flushTimer = nil;
    [_timerThread cancel];
    _timerThread = nil;
}

- (void)removeBackgroundListener {
    [[NSNotificationCenter defaultCenter] removeObserver:self name:UIApplicationDidEnterBackgroundNotification object:nil];
}
    
- (void)removeTerminateListener {
    [[NSNotificationCenter defaultCenter] removeObserver:self name:UIApplicationWillTerminateNotification object:nil];
}

- (void)flushForTimer {
    BigWriter tempWriter;
    tempWriter.flush();
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
    [[BListener sharedInstanced] addTimer];
}

void __removeListener() {
    [[BListener sharedInstanced] removeBackgroundListener];
    [[BListener sharedInstanced] removeTerminateListener];
    [[BListener sharedInstanced] removeTimer];
}
