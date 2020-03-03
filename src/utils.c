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
 
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "utils.h"

static log_write_t _write;

static const char *level_names[] = {
  "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
};

#ifdef LOG_USE_COLOR
static const char *level_colors[] = {
  "\x1b[94m", "\x1b[36m", "\x1b[32m", "\x1b[33m", "\x1b[31m", "\x1b[35m"
};
#endif  // LOG_USE_COLOR

void log_io_set(log_write_t writecb)
{
  _write = writecb;
}

void log_msg(int level, const char* fmt, ...)
{
  if (level >= MSG_TOTAL) {
    return;
  }
  
  va_list args;
  static uint8_t buffer[256];
  
#ifdef LOG_USE_COLOR
  snprintf(buffer, 256, "%s%-8s \x1b[0m", level_colors[level], 
           level_names[level]);
  _write(buffer, strlen(buffer));
#else
  snprintf(buffer, 256, "%-8s \x1b[0m", level_names[level]);
  _write(buffer, strlen(buffer));
#endif  // LOG_USE_COLOR

  va_start(args, fmt);
  vsnprintf(buffer, 256, fmt, args);
  va_end(args);
  
  _write(buffer, strlen(buffer));
}
