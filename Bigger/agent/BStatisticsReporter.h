//
//  BStatisticsReporter.h
//  Bigger
//
//  Created by 王仕杰 on 2017/8/3.
//

#ifndef _H_BStatisticsReporter_H_
#define _H_BStatisticsReporter_H_

#if defined(__cplusplus)
extern "C" {
#endif

void report_statistics_msg(const char * msg, const char * url, int headerSize, const char** headerField);

#if defined(__cplusplus)
}
#endif

#endif
