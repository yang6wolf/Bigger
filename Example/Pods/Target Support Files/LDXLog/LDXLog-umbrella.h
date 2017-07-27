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

#import "xloggerbase.h"
#import "xlogger.h"
#import "appender.h"

FOUNDATION_EXPORT double LDXLogVersionNumber;
FOUNDATION_EXPORT const unsigned char LDXLogVersionString[];

