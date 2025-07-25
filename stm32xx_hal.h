/* SPDX-License-Identifier: MIT */
/*!
 * \file stm32xx_hal.h
 * \brief HAL header file for STM32F4xx series.
 * \details This file includes the necessary headers and definitions for using
 * the HAL with STM32 devices. It defines the device-specific headers based
 * on the selected STM32xx device.
 * \version 0.1.0
 * \author Roy Ratcliffe
 */

#pragma once

/*
 * Include the appropriate device header file. This file should be included to
 * ensure that the correct device-specific definitions and functions are
 * available. The device header file is selected based on the defined STM32F4xx
 * device. If no device is defined, an error is raised to prompt the user to
 * select the appropriate device.
 */
#ifdef STM32F407xx
#include "stm32f4xx_hal.h"
#else
#error "Please define the STM32xx device in your project settings"
#endif
