/*---------------------------------------------------------------------------
 * Copyright (c) 2021 Arm Limited (or its affiliates). All rights reserved.
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
 *
 *      Name:    retarget_stdio.c
 *      Purpose: Retarget stdio to USART
 *
 *---------------------------------------------------------------------------*/

#include "RTE_Components.h"

#define RETARGET_IO_User_Stub

#if (defined(RTE_Compiler_IO_STDERR) && defined(RTE_Compiler_IO_STDERR_User)) || \
    (defined(RTE_Compiler_IO_STDIN)  && defined(RTE_Compiler_IO_STDIN_User))  || \
    (defined(RTE_Compiler_IO_STDOUT) && defined(RTE_Compiler_IO_STDOUT_User))

/* Prototypes */
int stdio_init     (void);
int stderr_putchar (int ch);
int stdout_putchar (int ch);
int stdin_getchar  (void);

#if !defined(RETARGET_IO_User_Stub)

#include "Driver_USART.h"

#define USART_DRV_NUM           0
#define USART_BAUDRATE          115200

#define _USART_Driver_(n)  Driver_USART##n
#define  USART_Driver_(n) _USART_Driver_(n)
 
extern ARM_DRIVER_USART  USART_Driver_(USART_DRV_NUM);
#define ptrUSART       (&USART_Driver_(USART_DRV_NUM))


/**
  Initialize stdio
 
  \return          0 on success, or -1 on error.
*/
int stdio_init (void) {
  int32_t status;
 
  status = ptrUSART->Initialize(NULL);
  if (status != ARM_DRIVER_OK) return (-1);
 
  status = ptrUSART->PowerControl(ARM_POWER_FULL);
  if (status != ARM_DRIVER_OK) return (-1);
 
  status = ptrUSART->Control(ARM_USART_MODE_ASYNCHRONOUS |
                             ARM_USART_DATA_BITS_8       |
                             ARM_USART_PARITY_NONE       |
                             ARM_USART_STOP_BITS_1       |
                             ARM_USART_FLOW_CONTROL_NONE,
                             USART_BAUDRATE);
  if (status != ARM_DRIVER_OK) return (-1);
 
  status = ptrUSART->Control(ARM_USART_CONTROL_RX, 1);
  if (status != ARM_DRIVER_OK) return (-1);
 
  return (0);
}


#if defined(RTE_Compiler_IO_STDERR) && defined(RTE_Compiler_IO_STDERR_User)
/**
  Put a character to the stderr
 
  \param[in]   ch  Character to output
  \return          The character written, or -1 on write error.
*/
int stderr_putchar (int ch) {
  uint8_t buf[1];
 
  buf[0] = (uint8_t)ch;
  if (ptrUSART->Send(buf, 1) != ARM_DRIVER_OK) {
    return (-1);
  }
  while (ptrUSART->GetTxCount() != 1);
  return (ch);
}
#endif

#if defined(RTE_Compiler_IO_STDIN)  && defined(RTE_Compiler_IO_STDIN_User)
/**
  Get a character from the stdio
 
  \return     The next character from the input, or -1 on read error.
*/
int stdin_getchar (void) {
  uint8_t buf[1];
 
  if (ptrUSART->Receive(buf, 1) != ARM_DRIVER_OK) {
    return (-1);
  }
  while (ptrUSART->GetRxCount() != 1);
  return (buf[0]);
}
#endif

#if defined(RTE_Compiler_IO_STDOUT) && defined(RTE_Compiler_IO_STDOUT_User)
/**
  Put a character to the stdout

  \param[in]   ch  Character to output
  \return          The character written, or -1 on write error.
*/
int stdout_putchar (int ch) {
  uint8_t buf[1];
 
  buf[0] = (uint8_t)ch;
  if (ptrUSART->Send(buf, 1) != ARM_DRIVER_OK) {
    return (-1);
  }
  while (ptrUSART->GetTxCount() != 1);
  return (ch);
}
#endif

#else /* defined(RETARGET_IO_User_Stub) */

#include <stdint.h>

int32_t ITM_SendChar (int32_t ch);
int32_t ITM_ReceiveChar (void);

/**
  Initialize stdio
 
  \return          0 on success, or -1 on error.
*/
int stdio_init (void) {
  return (-1);
}

#if defined(RTE_Compiler_IO_STDERR) && defined(RTE_Compiler_IO_STDERR_User)
/**
  Put a character to the stderr

  \param[in]   ch  Character to output
  \return          The character written, or -1 on write error.
*/
int stderr_putchar (int ch) {
  #warning "Using stderr_putchar stub"
  return (ITM_SendChar(ch));
  //return (-1);
}
#endif

#if defined(RTE_Compiler_IO_STDIN) && defined(RTE_Compiler_IO_STDIN_User)
/**
  Get a character from the stdio

  \return     The next character from the input, or -1 on read error.
*/
int stdin_getchar (void) {
  #warning "Using stdin_getchar stub"
  int32_t ch;

  do {
    ch = ITM_ReceiveChar();
  } while (ch == -1);
  return (ch);
  //return (-1);
}
#endif

#if defined(RTE_Compiler_IO_STDOUT) && defined(RTE_Compiler_IO_STDOUT_User)
/**
  Put a character to the stdout

  \param[in]   ch  Character to output
  \return          The character written, or -1 on write error.
*/
int stdout_putchar (int ch) {
  #warning "Using stdout_putchar stub"
  return (ITM_SendChar(ch));
  //return (-1);
}
#endif

#endif /* !defined(RETARGET_IO_User_Stub) */
#endif
