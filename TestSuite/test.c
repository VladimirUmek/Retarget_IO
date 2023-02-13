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

#include "main.h"
#include "test.h"

static const osThreadAttr_t tf_main_attr = {
  .name       = "TestRunner",
  .stack_size = MAIN_THREAD_STACK
};

/*-----------------------------------------------------------------------------
 *      Init test suite
 *----------------------------------------------------------------------------*/
__WEAK void TS_Init (void) {
}

/*-----------------------------------------------------------------------------
 *      Uninit test suite
 *----------------------------------------------------------------------------*/
__WEAK void TS_Uninit (void) {
}

/*-----------------------------------------------------------------------------
 *      Enable IRQ
 *----------------------------------------------------------------------------*/
__WEAK void EnableIRQ (int32_t irq_num) {
  (void)irq_num;
}

/*-----------------------------------------------------------------------------
 *      Disable IRQ
 *----------------------------------------------------------------------------*/
__WEAK void DisableIRQ (int32_t irq_num) {
  (void)irq_num;
}

/*-----------------------------------------------------------------------------
 *      Set Pending IRQ
 *----------------------------------------------------------------------------*/
__WEAK void SetPendingIRQ (int32_t irq_num) {
  (void)irq_num;
}

/*-----------------------------------------------------------------------------
 *      Test cases list
 *----------------------------------------------------------------------------*/
static const TEST_CASE TC_List[] = {
  TCD ( TC_malloc_1,                     TC_MALLOC_1_EN ),
  TCD ( TC_malloc_2,                     TC_MALLOC_2_EN ),

  TCD ( TC_fcvt_1,                       TC_FCVT_1_EN   ),

  TCD ( TC_fopen_1,                      TC_FOPEN_1_EN ),
  TCD ( TC_fopen_2,                      TC_FOPEN_2_EN ),
  TCD ( TC_fopen_3,                      TC_FOPEN_3_EN ),
  TCD ( TC_fopen_4,                      TC_FOPEN_4_EN ),
  TCD ( TC_fopen_5,                      TC_FOPEN_5_EN ),
  TCD ( TC_fopen_6,                      TC_FOPEN_6_EN ),

  TCD ( TC_fwrite_1,                     TC_FWRITE_1_EN ),
  
  TCD ( TC_fread_1,                      TC_FREAD_1_EN ),

  TCD ( TC_fseek_1,                      TC_FSEEK_1_EN ),
  TCD ( TC_fseek_2,                      TC_FSEEK_2_EN ),
  
  TCD ( TC_fgetpos_1,                    TC_FGETPOS_1_EN ),

  TCD ( TC_getchar_1,                    TC_GETCHAR_1_EN ),

  TCD ( TC_scanf_1,                      TC_SCANF_1_EN ),
  TCD ( TC_scanf_2,                      TC_SCANF_2_EN ),
//  TCD ( , ),
};

/*-----------------------------------------------------------------------------
 *      Test suite description
 *----------------------------------------------------------------------------*/
static TEST_SUITE ts = {
  __FILE__, __DATE__, __TIME__,
  "Test Suite",
  TS_Init,
  TS_Uninit,
  1,
  TC_List,
  sizeof(TC_List)/sizeof(TC_List[0])
};

/*---------------------------------------------------------------------------
 * Application initialization
 *---------------------------------------------------------------------------*/
void app_initialize (void) {

  /* Initialize CMSIS-RTOS2 */
  osKernelInitialize();

  /* Create test framework main function as a thread */
  osThreadNew((osThreadFunc_t)tf_main, &ts, &tf_main_attr);

  /* Start executing the test framework main function */
  osKernelStart();
}
