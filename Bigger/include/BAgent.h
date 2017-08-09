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

void openBigWriter(const char *_logPath);
void closeBigWriter();
    
void runCommand(const char * identifier);

#if defined(__cplusplus)
}
#endif
        
#endif /* BAgent_h */
