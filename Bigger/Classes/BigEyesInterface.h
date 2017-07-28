//
//  XlogInterface.hpp
//  Pods
//
//  Created by 杨志超 on 2017/7/27.
//
//

#ifndef BigEyesInterface_h
#define BigEyesInterface_h

#if defined(__cplusplus)
extern "C" {
#endif
    
void initBigEyes(const char *_logPath, const char *_filePrefix);
void openBigEyes(const char *_logPath, const char *_filePrefix);
void closeBigEyes();

#define INITB(path, prefix) initBigEyes(path, prefix)
#define OPENB(path, prefix) openBigEyes(path, prefix)
#define CLOSEB() closeBigEyes()

#if defined(__cplusplus)
}
#endif
#endif /* BigEyesInterface_h */
