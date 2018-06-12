//
//  LDViewController.m
//  Bigger
//
//  Created by 杨志超 on 07/27/2017.
//  Copyright (c) 2017 NetEase. All rights reserved.
//

#import "LDViewController.h"
#import "Bigger_Example-Swift.h"
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
    
    [[Foo new] logExample:@"Hello Swift"];
    
    //runCommand("Bigger-Example");
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}
- (IBAction)testButtonDemo:(UIButton *)sender {
    LOGF("button1 test");
    NSString* logPath = [[NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0] stringByAppendingString:@"/log"];
    char command[1024];
    NSString* p = [logPath stringByAppendingPathComponent:@"dailylog.plog"];
    sprintf(command, "LC \
            -uhttps://api.leancloud.cn/1.1/files/Bigger-some-file\
            -bhttps://api.leancloud.cn/1.1/classes/Bigger\
            -p%s\
            -h\"X-LC-Id: LVQdV4HdaL5WiQAbycKuMhot-gzGzoHsz\"\
            -h\"X-LC-Key: oQGg6Y8FNQaqAfDdzzHGY6PA\"",
            [p cStringUsingEncoding:NSUTF8StringEncoding]);
    bigger_run_command(command);
    //LOGF("fatal test");
}

@end
