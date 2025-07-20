/* SPDX-License-Identifier: MIT */
/*!
 * \file stm32xx_dfu.c
 * \brief Device Firmware Update (DFU) implementation for STM32 microcontrollers.
 * \copyright 2025, Roy Ratcliffe, Northumberland, United Kingdom
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sub-license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 *      The above copyright notice and this permission notice shall be included in all copies or substantial
 *      portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "stm32xx_dfu.h"

#include <stddef.h> /* for size_t */

/*!
 * \brief Write the same value to all registers in a block.
 * \details
 * This macro writes the same value to all registers in a contiguous block
 * of memory-mapped registers. It assumes that the registers are of the
 * same type and size.
 * \param _regs_ An array of registers.
 * \param _data_ The value to write to each register.
 */
#define WR_ALL_REGS(_regs_, _data_)                                            \
  do                                                                           \
    for (size_t addr = 0; addr < sizeof(_regs_) / sizeof((_regs_)[0]); addr++) \
      (_regs_)[addr] = (_data_);                                               \
  while (0)

void vJumpToDFU(const uint32_t *pulMSP_PC) {
  /*
   * Disable interrupts upfront.
   * This is essential to prevent any further scheduling or interrupts pre-empting
   * while transitioning to DFU mode.
   * The inlined `__disable_irq` operation disables all interrupts, ensuring that no
   * further interrupts can occur while the core is entering DFU mode.
   */
  __disable_irq();

  /*
   * Disable SysTick timer.
   * This is essential to prevent any further scheduling or timing operations
   * while in DFU mode.
   * The SysTick timer is typically used for system timing and task scheduling
   * in FreeRTOS, and disabling it is essential to prevent any further
   * scheduling or timing operations while in DFU mode.
   */
  SysTick->CTRL = 0x00000000UL;

  /*
   * Clear all interrupt enable registers.
   * This ensures that no interrupts can occur when the core enters DFU mode.
   */
  WR_ALL_REGS(NVIC->ICER, 0xffffffffUL);

  /*
   * Clear all pending interrupts.
   * This ensures that no pending interrupts can occur when the core enters DFU
   * mode after re-enabling interrupts.
   */
  WR_ALL_REGS(NVIC->ICPR, 0xffffffffUL);

  /*
   * Enable interrupts again.
   * This is necessary to allow the boot loader to handle any interrupts that
   * may occur during the DFU process.
   * It does not automatically re-enable interrupts, as the
   * Cortex-M core's global interrupt enable bit resets to the unmasked state.
   * Note that this does not re-enable the SysTick timer, which remains disabled.
   */
  __enable_irq();

  /*
   * Set the main stack pointer to the value provided in the pul parameter.
   * This is necessary to ensure that the core starts executing code from the
   * correct stack.
   */
  __set_MSP(pulMSP_PC[0]);

  /*
   * Jump to the DFU entry point.
   * The boot loader is responsible for handling the actual firmware update
   * process, including reading the new firmware, writing it to the
   * appropriate memory locations, and verifying its integrity.
   */
  ((void (*)(void))(pulMSP_PC[1]))();

  /*
   * Enter an infinite loop after jumping to the DFU entry point.
   * This is necessary to prevent the execution of any further code if the
   * DFU process returns.
   * The boot loader is expected to handle the firmware update process and
   * will not return to this function.
   */
  while (1)
    ;
}
