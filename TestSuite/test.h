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

#ifndef TEST_H__
#define TEST_H__

#include <stdint.h>
#include "tf_main.h"
#include "cmsis_os2.h"
#include "cmsis_compiler.h"

#include "RTE_Components.h"

#include "test_config.h"

/* Interrupt handling */
#define IRQ_A          0
#define IRQ_B          1

extern void EnableIRQ     (int32_t irq_num);
extern void DisableIRQ    (int32_t irq_num);
extern void SetPendingIRQ (int32_t irq_num);

extern void (*TST_IRQHandler_A)(void);
extern void (*TST_IRQHandler_B)(void);

#define TST_IRQHandler TST_IRQHandler_A

/* Validation helper functions */
extern int  cmsis_rv2 (void);
extern void TS_Init   (void);
extern void TS_Uninit (void);

/* Test cases */
extern void TC_malloc_1 (void);
extern void TC_malloc_2 (void);

extern void TC_fcvt_1 (void);

extern void TC_fopen_1 (void);
extern void TC_fopen_2 (void);
extern void TC_fopen_3 (void);
extern void TC_fopen_4 (void);
extern void TC_fopen_5 (void);
extern void TC_fopen_6 (void);

extern void TC_fwrite_1 (void);
extern void TC_fwrite_2 (void);
extern void TC_fwrite_3 (void);

extern void TC_fread_1 (void);

extern void TC_fseek_1 (void);
extern void TC_fseek_2 (void);

extern void TC_fgetpos_1 (void);

extern void TC_getchar_1 (void);

extern void TC_scanf_1 (void);
extern void TC_scanf_2 (void);

#endif /* TEST_H__ */
