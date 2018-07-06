//
//  BStatisticsReporter.c
//  Bigger
//
//  Created by 王仕杰 on 2017/8/3.
//

#include "curl/curl.h"
#include <stdlib.h>

void report_statistics_msg(char * msg, const char * url, int headerSize, const char** headerField) {
    struct curl_slist *headers = NULL;
    
    for (int i = 0; i < headerSize; i++) {
        headers = curl_slist_append(headers, headerField[i]);
    }
    
    
    CURL* curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, msg);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    
    free(msg);
    curl_slist_free_all(headers);
}
