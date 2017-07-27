# LDXLog

[![CI Status](http://img.shields.io/travis/杨志超/LDXLog.svg?style=flat)](https://travis-ci.org/杨志超/LDXLog)
[![Version](https://img.shields.io/cocoapods/v/LDXLog.svg?style=flat)](http://cocoapods.org/pods/LDXLog)
[![License](https://img.shields.io/cocoapods/l/LDXLog.svg?style=flat)](http://cocoapods.org/pods/LDXLog)
[![Platform](https://img.shields.io/cocoapods/p/LDXLog.svg?style=flat)](http://cocoapods.org/pods/LDXLog)

## Example

To run the example project, clone the repo, and run `pod install` from the Example directory first.

## Requirements

## Installation

LDXLog is available through [CocoaPods](http://cocoapods.org). To install
it, simply add the following line to your Podfile:

```ruby
pod "LDXLog"
```

## Author

杨志超, yangzhichao1@corp.netease.com

## License

LDXLog is available under the MIT license. See the LICENSE file for more info.

使用说明：

使用xlog之前先调用[[LDXlogManager sharedInstance] initXlog]初始化(日志存放目录可在初始化中修改)，然后调用openXlog开启xlog功能，closeXlog关闭xlog功能。

1.引用LDXlogManager.h写日志说明

在需要写日志的地方，先引用”LDXlogManager.h”，然后在写日志的地方根据不同的日志级别用LDXlogManager.h中的宏即可，
例如在debug环境中，用LOG_DEBUG(“test”, @“test1”);就生成一条日志。

具体DEMO在LDViewController.m中

下面是日志打印宏的示例。
LOG_DEBUG(TAG, @"I wanna say: %@", @"hello mars!");

LOG_INFO(TAG, @"I wanna say: %@", @"hello mars!");

LOG_WARNING(TAG, @"I wanna say: %@", @"hello mars!");

LOG_ERROR(TAG, @"I wanna say: %@", @"hello mars!");

TAG请自行定义


2.引用LDXLog/xlogger.h写日志说明

xlogger.h封装了多个写日志的接口，但是引用该文件得把.m文件变成.mm文件。

根据日志级别有xverbose2/xinfo2/xdebug2/xwran2/xerror2/xfatal2/xassert2...等接口。

xinfo_function()可以记录方法的出入栈。

具体DEMO在LDXlogDemoController.mm中

在使用的接口方面支持多种匹配方式：

类型安全检测方式：%s %d 。例如：xinfo2(“%s %d”, “test”, 1)
序号匹配的方式：%0 %1 。例如：xinfo2(TSF”%0 %1 %0”, “test”, 1)
智能匹配的懒人模式：% 。例如：xinfo2(TSF”% %_”, “test”, 1)


