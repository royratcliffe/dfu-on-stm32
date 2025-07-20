/* SPDX-License-Identifier: MIT */
/*!
 * \file stm32xx_dfu.h
 * \brief Device Firmware Update (DFU) interface for STM32 microcontrollers.
 * \author Roy Ratcliffe
 * \date 2025-07-20
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

#pragma once

#include <stdint.h> /* for uint32_t */

#include "stm32xx_hal.h" /* for HAL functions and definitions */

/*!
 * \brief Initiates Device Firmware Update (DFU) mode.
 * \param pulMSP_PC Pointer to the DFU entry point address. The first element should
 * be the main stack pointer value, and the second element should be the
 * address of the DFU entry point.
 *
 * Disables interrupts, clears all interrupt enable and pending registers, sets
 * the main stack pointer, and jumps to the DFU entry point. Intended to be
 * called when a DFU command is received. The function does not return; it will
 * enter DFU mode and never return to the caller.
 *
 * \note Uses the SysTick control register to disable the SysTick timer, which
 * is typically used for system timing and task scheduling in FreeRTOS.
 * Disabling it is \b essential to prevent any further scheduling or timing
 * operations while in DFU mode!
 * \note Uses the NVIC (Nested Vectored Interrupt Controller) to clear all
 * interrupt enable and pending registers. This ensures that no interrupts can
 * occur when the core enters DFU mode.
 * \note Jumps to the DFU entry point, which is expected to be located at the
 * address provided in the \c pulMSP_PC parameter. The boot loader is responsible for
 * handling the actual firmware update process, including reading the new
 * firmware, writing it to the appropriate memory locations, and verifying its
 * integrity.
 * \note Enters an infinite loop after jumping to the DFU entry point in order
 * to prevent the execution of any further code if the DFU process returns.
 * \warning Should only be called when the system is ready to enter DFU mode.
 * It will disable all interrupts and clear all interrupt enable and pending
 * registers, which may lead to loss of data or state if called at an
 * inappropriate time. Ensure that all necessary data is saved and that the
 * system is in a safe state before calling this function. Your milage may
 * vary.
 */
void vJumpToDFU(const uint32_t *pulMSP_PC) __attribute__((noreturn));
