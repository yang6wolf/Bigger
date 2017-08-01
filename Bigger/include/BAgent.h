//
//  BAgent.h
//  Pods
//
//  Created by Edward on 28/7/17.
//
//

#ifndef BAgent_h
#define BAgent_h

#if defined(__cplusplus)
extern "C" {
#endif
    
void printLogToConsole(int nType, int bEnable);

void initBigWriter(const char *_logPath, const char *_prefix, bool _isCompress, bool _isCrypt);
void initBigWriterWithType(const char *_logPath, const char *_prefix, bool _isCompress, bool _isCrypt, int nType);
void openBigWriter();
void openBigWriterWithType(int nType);
void closeBigWriter();
void flushBigWriter();
void syncFlushBigWriter();

#if defined(__cplusplus)
}
#endif
        
#endif /* BAgent_h */
