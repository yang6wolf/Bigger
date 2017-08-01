//
//  BTreadUtil.h
//  Pods
//
//  Created by Edward on 1/8/17.
//
//

#ifndef BThreadUtil_h
#define BThreadUtil_h

#if defined(__cplusplus)
extern "C" {
#endif
    
int64_t getCurrnetThreadID();

int64_t getMainThreadID();

#if defined(__cplusplus)
}
#endif
        
#endif /* BThreadUtil_h */
