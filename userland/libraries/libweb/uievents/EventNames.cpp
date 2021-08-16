/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/uievents/EventNames.h>

namespace Web::UIEvents::EventNames {

#define __ENUMERATE_UI_EVENT(name) FlyString name;
ENUMERATE_UI_EVENTS
#undef __ENUMERATE_UI_EVENT

[[gnu::constructor]] static void initialize()
{
    static bool s_initialized = false;
    if (s_initialized)
        return;

#define __ENUMERATE_UI_EVENT(name) \
    name = #name;
    ENUMERATE_UI_EVENTS
#undef __ENUMERATE_UI_EVENT

    s_initialized = true;
}

}