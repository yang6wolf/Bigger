// Tencent is pleased to support the open source community by making Mars available.
// Copyright (C) 2016 THL A29 Limited, a Tencent company. All rights reserved.

// Licensed under the MIT License (the "License"); you may not use this file except in 
// compliance with the License. You may obtain a copy of the License at
// http://opensource.org/licenses/MIT

// Unless required by applicable law or agreed to in writing, software distributed under the License is
// distributed on an "AS IS" basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
// either express or implied. See the License for the specific language governing permissions and
// limitations under the License.


/*
 * platform_comm.h
 *
 *  Created on: 2012-11-2
 *      Author: yerungui
 */

#ifndef COMM_PLATFORM_COMM_H_
#define COMM_PLATFORM_COMM_H_

#include <string>

#include "comm/thread/mutex.h"

#ifndef __cplusplus
#error "C++ only"
#endif


#ifdef __APPLE__
void FlushReachability();
float publiccomponent_GetSystemVersion();
#endif

#ifdef ANDROID
bool startAlarm(int64_t id, int after);
bool stopAlarm(int64_t id);

void* wakeupLock_new();
void  wakeupLock_delete(void* _object);
void  wakeupLock_Lock(void* _object);
void  wakeupLock_Lock_Timeout(void* _object, int64_t _timeout);
void  wakeupLock_Unlock(void* _object);
bool  wakeupLock_IsLocking(void* _object);
#endif

#ifdef ANDROID
	extern int g_NetInfo;
	extern WifiInfo g_wifi_info;
	extern SIMInfo g_sim_info;
	extern APNInfo g_apn_info;
	extern Mutex g_net_mutex;
#endif

#endif /* COMM_PLATFORM_COMM_H_ */
