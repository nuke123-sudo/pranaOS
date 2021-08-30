/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef _LIBOBJC_MEMORY_H
#define _LIBOBJC_MEMORY_H

// includes
#include <libobjc/runtime.h>
#include <stdlib.h>

#define objc_malloc(...) malloc(__VA_ARGS__)
#define objc_realloc(...) realloc(__VA_ARGS__)
#define objc_calloc(...) calloc(__VA_ARGS__)
#define objc_free(...) free(__VA_ARGS__)

id alloc_instance(Class cls);

#endif 
