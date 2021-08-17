/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef _KERNEL_DRIVERS_AARCH32_PL031_H
#define _KERNEL_DRIVERS_AARCH32_PL031_H

// includes
#include <drivers/driver_manager.h>
#include <platform/aarch32/interrupts.h>
#include <platform/aarch32/target/cortex-a15/device_settings.h>
#include <libkernel/types.h>
#include <libkernel/mask.h>

struct pl031_registers {
    uint32_t data;
    uint32_t match;
    uint32_t load;
    uint32_t control;
};
typedef struct pl031_registers pl031_registers_t;

void pl031_install();
uint32_t pl031_read_rtc();

#endif 