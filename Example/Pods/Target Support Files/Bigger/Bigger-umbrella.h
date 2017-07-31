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

FOUNDATION_EXPORT double BiggerVersionNumber;
FOUNDATION_EXPORT const unsigned char BiggerVersionString[];

