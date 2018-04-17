//
//  BiggerFileUploader.h
//  Bigger
//
//  Created by 王仕杰 on 2017/7/28.
//  Copyright © 2017年 NetEase. All rights reserved.
//
#if defined(__cplusplus)
extern "C" {
#endif
    
    void bigger_file_upload_APM(const char * url,
                                const char * file_path,
                                const char ** header,
                                int header_size,
                                const char ** arguments,
                                int args_size);
    
    void bigger_file_upload_LC(const char * file_url,
                               const char * bind_url,
                               const char * file_path,
                               const char ** header,
                               int header_size,
                               const char ** arguments,
                               int args_size);
    
#if defined(__cplusplus)
}
#endif
