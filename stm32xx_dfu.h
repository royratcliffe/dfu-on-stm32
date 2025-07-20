/*!
 * \file stm32xx_dfu.h
 * \brief Device Firmware Update (DFU) interface for STM32 microcontrollers.
 * \author Roy Ratcliffe
 * \date 2025-07-20
 */

#pragma once

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
void vJumpToDFU(uint32_t *pulMSP_PC) __attribute__((noreturn));
