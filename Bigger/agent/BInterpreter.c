//
//  BigInterpreter.cpp
//  Bigger
//
//  Created by 王仕杰 on 2017/8/1.
//
//

#include "BAgent.h"
#include "BFileUploader.h"
#include <string.h>
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include "nargv.h"

// Command: APM -u"https://url.to.upload"\
//              -p"/path/to/file"\
//              -h"Content-Type: application/json"\
//              -a"DeviceID=something"

// Command: LC -u"https://url.to.upload"\
//             -b https://url.to.bind \
//             -p"/path/to/file"\
//             -hContent-Type: application/json\
//             -a"DeviceID=something"
bool bigger_run_command(const char *pCommand) {
    NARGV* args;
    
    args = nargv_parse(pCommand);
    
    char * url;
    char * bind_url;
    char * path;
    char ** headers = calloc(50, sizeof(char *));
    int header_size = 0;
    char ** arguments = calloc(50, sizeof(char *));
    int args_size = 0;
    
    opterr = 1;
    int ch;
    while ((ch = getopt(args->argc, args->argv, "u:p:h:a:b:")) != -1) {
        switch (ch) {
            case 'a':
                arguments[args_size++] = optarg;
                printf("-a %s\n", optarg);
                break;
            case 'b':
                bind_url = optarg;
                printf("-b %s\n", optarg);
                break;
            case 'u':
                url = optarg;
                printf("-u %s\n", optarg);
                break;
            case 'p':
                path = optarg;
                printf("-p %s\n", optarg);
                break;
            case 'h':
                headers[header_size++] = optarg;
                printf("-h %s\n", optarg);
                break;
            case '?':
                printf("???\n");
                break;
            default:
                break;
        }
    }
    optind = 1;
    
    if (strstr(pCommand, "APM") == pCommand) {
        // "APM" is the first word in pCommand
        bigger_file_upload_APM(url, path, (const char **)headers, header_size, (const char **)arguments, args_size);
        
    } else if (strstr(pCommand, "LC") == pCommand) {
        // "LC" is the first word in pCommand (LeanCloud)
        bigger_file_upload_LC(url, bind_url, path, (const char **)headers, header_size, (const char **)arguments, args_size);
    }
    
    nargv_free(args);
    free(headers);
    free(arguments);
    
    return true;
}
