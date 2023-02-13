/*
 * Copyright (C) 2023 ARM Limited or its affiliates. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef RV2_CONFIG_H__
#define RV2_CONFIG_H__

//-------- <<< Use Configuration Wizard in Context Menu >>> --------------------

// <h> Test Suite Configuration
//   <o>Test Runner Thread Stack size <128-16384>
//   <i> Defines stack size for test runner thread.
//   <i> Default: 1024
#define MAIN_THREAD_STACK                 1024
// </h>

//   <o>Heap size
//   <i> Defines the size of heap memory available (in bytes).
#define HEAP_SIZE_TOTAL                   0x2000


#define TC_MALLOC_1_EN                    1
#define TC_MALLOC_2_EN                    1

#define TC_FCVT_1_EN                      0

#define TC_FOPEN_1_EN                     1
#define TC_FOPEN_2_EN                     1
#define TC_FOPEN_3_EN                     1
#define TC_FOPEN_4_EN                     1
#define TC_FOPEN_5_EN                     1
#define TC_FOPEN_6_EN                     1

#define TC_FWRITE_1_EN                    1

#define TC_FREAD_1_EN                     1

#define TC_FSEEK_1_EN                     1
#define TC_FSEEK_2_EN                     1

#define TC_FGETPOS_1_EN                   1

#define TC_GETCHAR_1_EN                   0

#define TC_SCANF_1_EN                     0
#define TC_SCANF_2_EN                     0


#endif /* RV2_CONFIG_H__ */
