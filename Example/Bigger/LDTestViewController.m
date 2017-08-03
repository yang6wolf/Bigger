//
//  LDTestViewController.m
//  Bigger
//
//  Created by 杨志超 on 2017/7/31.
//  Copyright © 2017年 杨志超. All rights reserved.
//

#import "LDTestViewController.h"
@import Bigger;

@interface LDTestViewController ()

@end

@implementation LDTestViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}
- (IBAction)testButtonDemo:(UIButton *)sender {
    LOGI("button2 test");
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
