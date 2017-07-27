//
//  LDViewController.m
//  Bigger
//
//  Created by 杨志超 on 07/27/2017.
//  Copyright (c) 2017 杨志超. All rights reserved.
//

#import "LDViewController.h"
#import "Bigger.h"

@interface LDViewController ()

@end

@implementation LDViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
    
    //LOGE("test");
    NSString* logPath = [[NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0] stringByAppendingString:@"/log"];
    //INITXLOG(logPath);
    INITX([logPath UTF8String], "LDPM");
    NSLog(@"logPath : %@", logPath);
    LOGE("疯狂测试!!!!!!");
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
