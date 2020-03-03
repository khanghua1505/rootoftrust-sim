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
#include <string.h>

#include "sha3.h"
#include "ed25519.h"
#include "keycore.h"

#define KEYCORE_STATUS_INIT             (0x0000)
#define KEYCORE_STATUS_SEC_KEYS_FLAG    (0x01 << 1)
#define KEYCORE_STATUS_HASHCODE_FLAG    (0x01 << 2)
#define KEYCORE_STATUS_SIGNATURE_FLAG   (0x01 << 3)

static keycore_status_t m_status = KEYCORE_STATUS_INIT;
static sha3_ctx_t       m_SHA3ctx;

bool keycore_hashcode_isvalid(void)
{
  return (m_status & KEYCORE_STATUS_HASHCODE_FLAG);
}

bool keycore_seckeys_isvalid(void)
{
  return (m_status & KEYCORE_STATUS_SEC_KEYS_FLAG);
}

void keycore_payload_init(void)
{
  sha3_init(&m_SHA3ctx, 64);
}

void keycore_payload_append(const uint8_t *buffer, uint8_t size)
{
  sha3_update(&m_SHA3ctx, buffer, size);
}

void keycore_payload_final(void)
{
  uint8_t scratchpad[128];
  
  sha3_final(sec_hash, &m_SHA3ctx);
  m_status |= KEYCORE_STATUS_HASHCODE_FLAG;
  
  sha3_init(&m_SHA3ctx, 64);
  sha3_update(&m_SHA3ctx, dev_prv_key, sizeof(dev_prv_key));
  sha3_update(&m_SHA3ctx, sec_hash, sizeof(sec_hash));
  sha3_final(scratchpad, &m_SHA3ctx);
  
  ed25519_create_keypair(sec_pub_key, sec_prv_key, scratchpad);
  m_status |= KEYCORE_STATUS_SEC_KEYS_FLAG;
  
  memcpy(scratchpad, sec_hash, 64);
  memcpy(scratchpad + 64, sec_pub_key, 32);
  
  ed25519_sign(sec_signa, scratchpad, 96, dev_pub_key, dev_prv_key);
  m_status |= KEYCORE_STATUS_SIGNATURE_FLAG;
  
  memset(scratchpad, 0, 128);
}

void keycore_signature_msg_init(void)
{
  sha3_init(&m_SHA3ctx, 64);
}

void keycore_signature_msg_append(const uint8_t *buffer, uint8_t size)
{
  sha3_update(&m_SHA3ctx, buffer, size);
}

void keycore_signature_msg_final(uint8_t *signature)
{
  uint8_t scratchpad[128];
  uint8_t tmp[64];
  
  sha3_final(tmp, &m_SHA3ctx);
  memcpy(scratchpad, tmp, 64);
  memcpy(scratchpad + 64, sec_pub_key, 32);
  
  ed25519_sign(signature, scratchpad, 96, dev_pub_key, dev_prv_key);
}







