//
//  BiggerFileUploader.c
//  Bigger
//
//  Created by 王仕杰 on 2017/7/28.
//  Copyright © 2017年 NetEase. All rights reserved.
//

#include "BFileUploader.h"

#include "curl/curl.h"
#include "cJSON.h"
#include "BLoggerWrapper.h"

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>

void bigger_file_upload_APM(const char * url,
                            const char * file_path,
                            const char ** header,
                            int header_size,
                            const char ** arguments,
                            int args_size) {
    
    assert(url);
    assert(args_size > 0);
    
    // A form-data post
    
    CURL * curl = curl_easy_init();
    
    if (curl) {
        // http header
        struct curl_slist *http_headers = NULL;
        for (int i = 0; i < header_size; i++) {
            http_headers = curl_slist_append(http_headers, header[i]);
        }
        
        /*
         * If the size of HTTP body ≥ 1024
         * libCurl will send a `Expect: 100-continue` in HTTP header
         * If the server does not support this feature
         * Uncomment the lines below
         */
//        http_headers = curl_slist_append(http_headers, "Expect: ");
        
        
        curl_mime * mime = curl_mime_init(curl);
        
        // form args
        for (int i = 0; i < args_size; i++) {
            char * arg = strdup(arguments[i]);
            char * token = strtok(arg, "=");
            
            curl_mimepart* part = curl_mime_addpart(mime);
            curl_mime_name(part, token);
            token = strtok(NULL, "=");
            curl_mime_data(part, token, CURL_ZERO_TERMINATED);
            
            free(arg);
        }
        
        // file
        if (file_path) {
            curl_mimepart* part = curl_mime_addpart(mime);
            curl_mime_name(part, "file");
            curl_mime_filedata(part, file_path);
        }
        
        // curl config
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, http_headers);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        
        curl_easy_perform(curl);
        
        curl_mime_free(mime);
        curl_slist_free_all(http_headers);
        curl_easy_cleanup(curl);
    }
}

static size_t WriteCallback(char *contents, size_t size, size_t nmemb, void **userp) {
    *userp = malloc(size * nmemb);
    strcpy(*userp, contents);
    return size * nmemb;
}

void bigger_file_upload_LC(const char * file_url,
                           const char * bind_url,
                           const char * file_path,
                           const char ** header,
                           int header_size,
                           const char ** arguments,
                           int args_size) {
    // POST the file's content first, then bind the info.
    assert(file_url);
    assert(file_path);
    
    CURL* curl = curl_easy_init();
    
    if (curl) {
        // file
        FILE* file = fopen(file_path, "rb");
        
        if (!file) {
            LOGF("Cannot open file at path %s", file_path);
            return;
        }
        
        struct stat file_info;
        stat(file_path, &file_info);
        
        // http header
        struct curl_slist *http_headers = NULL;
        for (int i = 0; i < header_size; i++) {
            http_headers = curl_slist_append(http_headers, header[i]);
        }
        
        http_headers = curl_slist_append(http_headers, "Content-Type: application/octet-stream");
        
        char arg_length_header[100];
        sprintf(arg_length_header, "Content-Length: %lld", file_info.st_size);
        http_headers = curl_slist_append(http_headers, arg_length_header);
        
        // response
        char* response;
        
        // curl config
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_URL, file_url);
        curl_easy_setopt(curl, CURLOPT_POST, 1);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, http_headers);
        curl_easy_setopt(curl, CURLOPT_READDATA, file);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, *WriteCallback);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        
        CURLcode ret = curl_easy_perform(curl);
        
        // parse response
        cJSON* response_root = cJSON_Parse(response);
        cJSON* id_obj = cJSON_GetObjectItem(response_root, "objectId");
        char * id_string = cJSON_GetStringValue(id_obj);
        
        if (ret == CURLE_OK && id_string) {
            // bind file body
            cJSON* request_root = cJSON_CreateObject();
            
            // add arguments to json body
            for (int i = 0; i < args_size; i++) {
                char * arg = strdup(arguments[i]);
                char* token = strtok(arg, "=");
                
                cJSON_AddStringToObject(request_root, token, strtok(NULL, "="));
                
                free(arg);
            }
            
            cJSON* log_file_obj = cJSON_AddObjectToObject(request_root, "logFile");
            cJSON_AddStringToObject(log_file_obj, "id", id_string);
            cJSON_AddStringToObject(log_file_obj, "__type", "File");
            
            char * body_string = cJSON_Print(request_root);
            printf("body string is:\n%s\n", body_string);
            
            struct curl_slist *bind_headers = NULL;
            for (int i = 0; i < header_size; i++) {
                bind_headers = curl_slist_append(bind_headers, header[i]);
            }
            bind_headers = curl_slist_append(bind_headers, "Content-Type: application/json");
            
            curl_easy_reset(curl);
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, bind_headers);
            curl_easy_setopt(curl, CURLOPT_URL, bind_url);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body_string);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
            
            curl_easy_perform(curl);
            
            cJSON_free(body_string);
            curl_slist_free_all(bind_headers);
            cJSON_Delete(request_root);
        }
        
        cJSON_Delete(response_root);
        curl_slist_free_all(http_headers);
        curl_easy_cleanup(curl);
        free(response);
        
    }
    
}
