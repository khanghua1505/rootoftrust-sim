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
 
#ifndef _ROOT_OF_TRUST_UTILS_H_
#define _ROOT_OF_TRUST_UTILS_H_

#include <stdint.h>

#define GET32(a, b, c, d)  ( ((a) << 24) | ((b) << 16) |  \
                             ((c) << 8) | ((d)) )
                             
#define LOG_USE_COLOR   1
 
enum { MSG_TRACE, MSG_DEBUG, MSG_INFO, MSG_WARN, MSG_ERROR, MSG_FATAL, MSG_TOTAL };

#define msg_trace(...)  log_msg(MSG_TRACE, __VA_ARGS__)
#define msg_debug(...)  log_msg(MSG_DEBUG, __VA_ARGS__)
#define msg_info(...)   log_msg(MSG_INFO, __VA_ARGS__)
#define msg_warn(...)   log_msg(MSG_WARN, __VA_ARGS__)
#define msg_error(...)  log_msg(MSG_ERROR, __VA_ARGS__)
#define msg_fatal(...)  log_msg(MSG_FATAL, __VA_ARGS__)

typedef void (*log_write_t)(const uint8_t *buffer, size_t len);

void log_io_set(log_write_t writecb);

void log_msg(int level, const char* fmt, ...);

#endif  // _ROOT_OF_TRUST_UTILS_H_



