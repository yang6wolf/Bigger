//
//  LDViewController.m
//  Bigger
//
//  Created by 杨志超 on 07/27/2017.
//  Copyright (c) 2017 NetEase. All rights reserved.
//

#import "LDViewController.h"
//#import "Bigger.h"

@import Bigger;

@interface LDViewController ()

@end

@implementation LDViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
    
    //printLogToConsole(B_LOG_TYPE_ERROR | B_LOG_TYPE_ERROR, 1);

    LOGI("界面加载完成！");
    
    //runCommand("Bigger-Example");
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}
- (IBAction)testButtonDemo:(UIButton *)sender {
    LOGI("button1 test");
    //LOGF("fatal test");
}

@end
