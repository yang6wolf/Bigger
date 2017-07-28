//
//  XlogInterface.hpp
//  Pods
//
//  Created by 杨志超 on 2017/7/27.
//
//

#ifndef BigWriterInterface_h
#define BigWriterInterface_h

#if defined(__cplusplus)
extern "C" {
#endif
    
void initBigWriter(const char *_logPath, const char *_filePrefix);
void openBigWriter(const char *_logPath, const char *_filePrefix);
void closeBigWriter();

#define INITB(path, prefix) initBigWriter(path, prefix)
#define OPENB(path, prefix) openBigWriter(path, prefix)
#define CLOSEB() closeBigWriter()

#if defined(__cplusplus)
}
#endif
#endif /* XlogInterface_h */
