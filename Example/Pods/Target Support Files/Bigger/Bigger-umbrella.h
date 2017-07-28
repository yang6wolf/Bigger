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

#import "BCommonDef.h"
#import "Bigger.h"
#import "BLoggerInterface.h"
#import "XlogInterface.h"

FOUNDATION_EXPORT double BiggerVersionNumber;
FOUNDATION_EXPORT const unsigned char BiggerVersionString[];

