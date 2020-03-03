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

#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>
#include "utils.h"

#ifdef CONFIG_INTERFACE_SERIAL
#  include "serial.h"
#elif defined(CONFIG_INTERFACE_ETHERNET)
#  include "ethernet.h"
#endif

#ifdef CONFIG_INTERFACE_SERIAL

#elif defined(CONFIG_INTERFACE_ETHERNET)
void __write_byte(uint8_t c) 
{
  ssize_t byte_transferred;
  
  byte_transferred = socket_send(&c, 1);
  if (byte_transferred != 1) {
    msg_error("Byte is not written\n");
  }
}

int __read_byte(void)
{
  uint8_t c;
  ssize_t byte_transferred;
  
  byte_transferred = socket_read(&c, 1);
  if (byte_transferred != 1) {
    msg_error("Byte is not read\n");
    return -1;
  }
  
  return (int) c;
}

void __write(const uint8_t *buffer, uint16_t size)
{
  ssize_t byte_transferred;
  
  byte_transferred = socket_send(buffer, size);
  if (byte_transferred != size) {
    msg_error("Bytes is not written, byte_transferred=%d\n", byte_transferred);
  }
}

void __read(uint8_t *buffer, uint16_t size) 
{
  ssize_t byte_transferred;
  
  byte_transferred = socket_read(buffer, size);
  if (byte_transferred != size) {
    msg_error("Bytes is not read, byte_transferred=%d\n", byte_transferred);
  }
}
#endif


  
