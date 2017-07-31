#ifdef __OBJC__
#import <UIKit/UIKit.h>
#else
#ifndef FOUNDATION_EXPORT
#if defined(__cplusplus)
#define FOUNDATION_EXPORT extern "C"
#else
#define FOUNDATION_EXPORT extern
#endif
#endif
#endif

#import "BAgent.h"
#import "BCommonDef.h"
#import "BigEyesInterface.h"
#import "Bigger.h"
#import "BLoggerWrapper.h"
<<<<<<< HEAD
=======
#import "BAgent.h"
>>>>>>> 给filewrite的开启接口添加是否加密参数，对Example工程补充了Demo View。

FOUNDATION_EXPORT double BiggerVersionNumber;
FOUNDATION_EXPORT const unsigned char BiggerVersionString[];

