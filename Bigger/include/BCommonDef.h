//
//  BCommonDef.h
//  Bigger
//
//  Created by Edward on 27/7/17.
//  Copyright © 2017年 Netease. All rights reserved.
//

#ifndef BCommonDef_h
#define BCommonDef_h

typedef enum BLogType{
    B_LOG_TYPE_FATAL=1,
    B_LOG_TYPE_ERROR=1<<4,
    B_LOG_TYPE_INFO=1<<8,
    B_LOG_TYPE_DEBUG=1<<16
}BLogType;


#endif /* BCommonDef_h */
