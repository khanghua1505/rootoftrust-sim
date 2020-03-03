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

#ifndef _ROOT_OF_TRUST_IO_H_
#define _ROOT_OF_TRUST_IO_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

/**
  * @brief This function sends a character to STDOUT.
  *
  * @param c[in]    An input character.
  *
  * @return None.
  */
void __write_byte(uint8_t c);

/**
  * @brief This function receives a character from STDIN.
  *
  * @param None.
  *
  * @return The received character.
  */
int __read_byte(void);

/**
  * @brief This function sends a string (or byte array) to STDOUT.
  *
  * @param buffer[in]    An input string or (byte array).
  * @param buffer[in]    The size of buffer.
  *
  * @return None.
  */
void __write(const uint8_t *buffer, uint16_t size);

/**
  * @brief This function sends a string (or byte array) to STDOUT.
  *
  * @param buffer[in,out]    The buffer to store received datas.
  * @param buffer[in]        Number of bytes to shall be read.
  *
  * @return None.
  */
void __read(uint8_t *buffer, uint16_t size);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // _ROOT_OF_TRUST_IO_H__
