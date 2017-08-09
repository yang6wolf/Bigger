//
//  LDAppDelegate.m
//  Bigger
//
//  Created by 杨志超 on 07/27/2017.
//  Copyright (c) 2017 NetEase. All rights reserved.
//

#import "LDAppDelegate.h"

@import Bigger;

@implementation LDAppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    printLogToConsole(0xFFFFFFFF, 1);
    
    ILOGD(1==1, "Demonstration of %s ", "ILOG");
    CHECK(1==1, LOGE("Try to use CHECK macro %s", "^_^"));
    
    NSArray *testArray=[NSArray arrayWithObjects:@"Shanghai",@"Beijing",@"Guangzhou",@"New York",@"Paris", nil];
    
    LOGD(NSCHAR(@"testArr is %@", testArray));
    NSLOGD(@"testArr is %@", testArray);
    CHECK(1==1, NSLOGD(@"testArr is %@", testArray));
    
    NSLog(@"NSLog output.");
    NSString* logPath = [[NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0] stringByAppendingString:@"/log"];
    
#warning StatisticsMonitor Closed
//    initStatisticsMonitor();
    
    
    openBigWriter([logPath UTF8String]);
    NSLog(@"logPath : %@", logPath);
//    LOGE("启动完成!");
    
    LOGE("%@", launchOptions);
    
    
    
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
    closeBigWriter();
}

@end
