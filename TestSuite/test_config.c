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

#include "test.h"
#include "test_config_device.h"

#if defined(RTE_Compiler_IO_File)
  #if defined(RTE_Compiler_IO_File_MDKFS)
    #include "rl_fs.h"
  #endif
#endif

/* Prototypes */
void TST_IRQ_HANDLER_A (void);
void TST_IRQ_HANDLER_B (void);
void (*TST_IRQHandler_A)(void);
void (*TST_IRQHandler_B)(void);

extern int stdout_putchar (int ch);

/*
  Primary interrupt handler
*/
void TST_IRQ_HANDLER_A (void) {

  if (TST_IRQHandler_A != NULL) {
    TST_IRQHandler_A();
  }
}

/*
  Secondary interrupt handler
*/
void TST_IRQ_HANDLER_B (void) {

  if (TST_IRQHandler_B != NULL) {
    TST_IRQHandler_B();
  }
}

/*
  Test suite initialization
*/
void TS_Init (void) {
  /* Set primary and secondary interrupt priority */
  NVIC_SetPriority ((IRQn_Type)TST_IRQ_NUM_A, 5U);
  NVIC_SetPriority ((IRQn_Type)TST_IRQ_NUM_B, 4U);

  /* Enable interrupts */
  NVIC_EnableIRQ((IRQn_Type)TST_IRQ_NUM_A);
  NVIC_EnableIRQ((IRQn_Type)TST_IRQ_NUM_B);

#if defined(RTE_Compiler_IO_File)
  #if defined(RTE_Compiler_IO_File_MDKFS)
  finit("R0");
  fmount("R0");
  fformat("R0", NULL);
  #endif
#endif
}

/*
  Test suite de-initialization
*/
void TS_Uninit (void) {
  /* Close debug session here */

  /* Note:
     VHT model shall have parameter shutdown_on_eot set to true.
     Simulation is then shutdown when EOT, ASCII4, character is
     transmitted via UART.
   */
  stdout_putchar (0x04);
}

/*
  Enable interrupt trigger in the IRQ controller.
*/
void EnableIRQ (int32_t irq_num) {

  if (irq_num == IRQ_A) {
    irq_num = TST_IRQ_NUM_A;
  } else {
    irq_num = TST_IRQ_NUM_B;
  }

  NVIC_EnableIRQ((IRQn_Type)irq_num);
}

/*
  Disable interrupt trigger in the IRQ controller.
*/
void DisableIRQ (int32_t irq_num) {

  if (irq_num == IRQ_A) {
    irq_num = TST_IRQ_NUM_A;
  } else {
    irq_num = TST_IRQ_NUM_B;
  }

  NVIC_DisableIRQ((IRQn_Type)irq_num);
}

/*
  Set pending interrupt in the IRQ controller.
*/
void SetPendingIRQ (int32_t irq_num) {

  if (irq_num == IRQ_A) {
    irq_num = TST_IRQ_NUM_A;
  } else {
    irq_num = TST_IRQ_NUM_B;
  }

  NVIC_SetPendingIRQ((IRQn_Type)irq_num);

  __DSB();
  __ISB();
  __DMB();

  while (NVIC_GetPendingIRQ((IRQn_Type)irq_num) != 0);
}
