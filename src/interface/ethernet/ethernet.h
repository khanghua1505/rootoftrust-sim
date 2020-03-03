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
 
 #ifndef _KEYCORE_INF_ETHERNET_H_
 #define _KEYCORE_INF_ETHERNET_H_
 
 #include <stdint.h>
 #include <stddef.h>
 
 int socket_open();
 ssize_t socket_read(uint8_t *buffer, uint16_t len);
 ssize_t socket_send(const uint8_t *buffer, uint16_t len);
 
 #endif  //  _KEYCORE_INF_ETHERNET_H_
 
