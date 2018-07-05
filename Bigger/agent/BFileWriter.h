//
//  BFileWriter.h
//  Bigger
//
//  Created by 杨志超 on 2017/7/27.
//  Copyright © 2017年 NetEase. All rights reserved.
//

#ifndef BigWriter_h
#define BigWriter_h

#include <string>
#include "BLogger.h"
#include "appender.h"

class BigWriter : public BLogMonitor {
public:
    void Callback(BLogType eLogType, const char *pLog);
    
    void init(const char *_logPath, bool isCompress, bool isCrypt);
    void open(const char * filename);
    void close();
    void flush();
    void syncFlush();
    
    const char* getPath();
    const char* getFilename();
    bool isPathNull();
    
    void setMonitorType(int _type) {_MonitorType = _type;};
    void setRegister(bool _isRegister) {isRegister = _isRegister;};
    bool getRegister() {return isRegister;};
    bool getCrypt() {return isCrypt;};
    bool getCompress() {return isCompress;};
    
    BigWriter(): isRegister(false), isCompress(false), isCrypt(false) {
        _MonitorType = B_LOG_TYPE_FATAL | B_LOG_TYPE_ERROR | B_LOG_TYPE_INFO | B_LOG_TYPE_DEBUG;
        _MonitorID = arc4random();
    }
    
    virtual ~BigWriter() {
    }
private:
    Appender * appender = NULL;
    std::string logPath;
    bool isRegister;
    bool isCompress;
    bool isCrypt;
};
#endif /* BigWriter_h */
