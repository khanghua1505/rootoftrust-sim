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

#ifndef KEYCORE_H__
#define KEYCORE_H__

#include <stdint.h>
#include <stdbool.h>
#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#define DEV_PUBKEY_SIZE     (32)
#define DEV_PRVKEY_SIZE     (64)
#define SEC_PUBKEY_SIZE     (32)
#define SEC_PRVKEY_SIZE     (64)
#define SEC_HASH_SIZE       (64)
#define SEC_SIGNA_SIZE      (64)

extern uint8_t dev_pub_key[32];
extern uint8_t dev_prv_key[64];
extern uint8_t sec_pub_key[32];
extern uint8_t sec_prv_key[64];
extern uint8_t sec_hash[64];
extern uint8_t sec_signa[64];

#define VERSION(major, minor)           ( (major << 5) | minor )
#define CERBERUS_KEYCORE_VERSION        ( VERSION(0,1))
    
#define PID(a, b)                       {a, b}
#define CERBERUS_KEYCORE_PID            PID(0x04, 0xEE)
    
typedef uint32_t keycore_status_t;

#define KEYCORE_SEG_START       0x00
#define KEYCORE_SEG_NEXT        0x01
#define KEYCORE_SEG_LAST        0x03
typedef struct __packed
{
    uint8_t type;       /* Segment type:
                            - 0x00 : Start Segment.
                            - 0x01 : Next Segment.
                            - 0x03 : Last segment.
                        */
    uint8_t data[];     /* Received data and a checksum. */
}
keycore_data_segment_t;

/**
  * @brief This function is used to check the security keys is available.
  * 
  * @return true  If the security keys is valid.
  * @return false If the security keys is not available.
  */
bool keycore_seckeys_isvalid(void);

/**
  * @brief        This function is used to check the hashcode is available.
  * 
  * @retval true  If the hashcode is valid.
  * @retval false if the hashcode is not available.
  */
bool keycore_hashcode_isvalid(void);
      
void keycore_payload_init(void);

void keycore_payload_append(const uint8_t *buffer, uint8_t size);

void keycore_payload_final(void);

void keycore_signature_msg_init(void);

void keycore_signature_msg_append(const uint8_t *buffer, 
             uint8_t size);

void keycore_signature_msg_final(uint8_t *signature);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // KEYCORE_H__
