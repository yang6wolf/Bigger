//
//  XlogInterface.hpp
//  Pods
//
//  Created by 杨志超 on 2017/7/27.
//
//

#ifndef XlogInterface_h
#define XlogInterface_h

#if defined(__cplusplus)
extern "C" {
#endif
    
void initXlog(const char *_logDir, const char *_filePrefix);
void openXlog(const char *_logDir, const char *_filePrefix);
void closeXlog();

#define INITX(path, prefix) initXlog(path, prefix)
#define OPENX(path, prefix) openXlog(path, prefix)
#define CLOSEX() closeXlog()

#if defined(__cplusplus)
}
#endif
#endif /* XlogInterface_h */
