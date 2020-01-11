/* Copyright (c) 2019 - 2020, Khang Hua, email: khanghua1505@gmail.com
 * All right reserved.
 *
 * This file is written and modified by Khang Hua.
 *
 * This model is free software; you can redistribute it and/or modify it under the terms
 * of the GNU Lesser General Public License; either version 2.1 of the License, or (at your option)
 * any later version. See the GNU Lesser General Public License for more details,
 *
 * This model is distributed in the hope that it will be useful.
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>

#ifndef __packed
#define __packed	__attribute__((packed))
#endif  // __packed

#ifndef __weak
#define __weak		__attribute__((weak))
#endif  // __weak

#ifndef _STATIC_ASSERT
#define _STATIC_ASSERT(COND, MSG)	typedef char static_assertion_##MSG[(!!(COND))*2-1]
#endif  // _STATIC_ASSERT

#ifndef STATIC_ASSERT
#define STATIC_ASSERT(COND)		_STATIC_ASSERT(COND, __LINE__);
#endif  // STATIC_ASSERT

#define KEYCORE_CHECK(module_enabled)	(module_enabled)

#define debug(fmt...)	printf(fmt)

#endif // CONFIG_H
