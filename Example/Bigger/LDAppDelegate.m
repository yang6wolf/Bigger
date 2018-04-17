//
//  LDAppDelegate.m
//  Bigger
//
//  Created by 杨志超 on 07/27/2017.
//  Copyright (c) 2017 NetEase. All rights reserved.
//

#import "LDAppDelegate.h"

@import Bigger;
#import "Bigger_Example-Swift.h"

@implementation LDAppDelegate

const char * foo(const char * bar) {
    char* s = malloc(strlen(bar) + 20);
    sprintf(s, "{\"log\" : \"%s\"}", bar);
    return s;
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    NSLOGD(@"%@", @"%3D%26n");
    
    bigger_init_sdk("BiggerExample-iOS", [[[[UIDevice currentDevice] identifierForVendor] UUIDString]UTF8String]);
    bigger_print_to_console(B_LOG_TYPE_INFO | B_LOG_TYPE_DEBUG | B_LOG_TYPE_ERROR | B_LOG_TYPE_FATAL, 1);
    LOGI("Print all levels of message to console");
    
    bigger_start_realtime_report(B_LOG_TYPE_FATAL, "http://zwwdata.ms.netease.com:8080", 0, NULL, NULL);

    ILOGD(1==1, "Demonstration of %s ", "ILOG");
    CHECK(1==1, LOGE("Try to use CHECK macro %s", "^_^"));
    
    NSArray *testArray = @[@"上海", @"서울", @"つくば", @"New York", @"Москва", @"👨‍👩‍👧‍👦"];
    NSLOGD(@"Test array is %@", testArray);
    
    NSString *testStr = @"招远 🎱";
    NSLOGD(@"Test string is %@", testStr);
    
    NSLog(@"NSLog output.");
    
    
    
    char* headers[3] = {"hello: world", "foo: bar", "Content-Type: application/json"};
    bigger_start_realtime_report(B_LOG_TYPE_FATAL, "https://httpbin.org/post", 3, (const char **)headers, foo);
    LOGI("\nStart report FATAL log to url: %s\n", "https://httpbin.org/post");
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(3 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        bigger_end_realtime_report("https://httpbin.org/post");
        [[Foo new] logExample:@"Remove the reporter with url (https://httpbin.org/post), should not see the realtime upload message"];
    });
    
    
    NSString* logPath = [[NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0] stringByAppendingString:@"/log"];
    bigger_start_write_log(B_LOG_TYPE_DEBUG, [logPath UTF8String]);
    NSLog(@"logPath : %@", logPath);
    
    LOGI("Hello kibana!");
    NSLOGE(@"%@", launchOptions);
    
    NSString *strKeyInfo = @"测试数据持久化";
    const char *pStrKeyInfo = [strKeyInfo UTF8String];
    persistentWrite((void *)pStrKeyInfo, (int)strlen(pStrKeyInfo));
    
    
    void *pRetrieve = NULL;
    int nLen = 0;
    persistentRead(&pRetrieve, &nLen);
    printf("\n%s\n", (char *)pRetrieve);
    
    persistentClear(nLen%5==0);
    
    // Override point for customization after application launch.
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
    bigger_end_write_log();
}

@end
