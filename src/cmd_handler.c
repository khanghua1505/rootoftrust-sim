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
#include <stdbool.h>
#include "utils.h"
#include "io.h"
#include "package.h"
#include "keycore.h"
#include "cmd_handler.h"
#include "config.h"

static void print_hex(const uint8_t *stream, uint16_t size)
{
  for (int i = 0; i < size; i += 8) {
    printf("           ");
    for (int j = 0; j < 8; j++) {
      printf("0x%2x ", stream[i + j]);
    }
    printf("\r\n");
  }
}

/*
 * The Get command.
 *
 * The Get command allows the user to get the version of the Cerberus Core
 * and the supported commands.
 */

static const get_return_pkt_t get_return_pkt = 
{
  .N = 9,                                 /* Number of bytes to be followed - 1. */
  .version = CERBERUS_KEYCORE_VERSION,    /* Keycore version. */
  .supported_commands = 
  {
    0x00,       /* Get command. */
    0x01,       /* Get Version command. */
    0x02,       /* Read Protection Status command. */
    0x03,       /* Get ID command. */
    0x11,       /* Read Device Public Key command. */
    0x12,       /* Read Hash Code command. */
    0x13,       /* Read Security Monitor Public Key command. */
    0x21,       /* Generate command. */
    0x31,       /* Sign command. */
  },
};

static void handle_get(void) 
{
  __write_byte(ACK);                            
  __write((uint8_t *) &get_return_pkt, sizeof(get_return_pkt));       
  __write_byte(ACK);                            
}

/*
 * Get version command.
 *
 * The Get version command is used to get the Keycore version.
 */

static const get_version_return_pkt_t get_version_return_pkt = 
{
  .version = CERBERUS_KEYCORE_VERSION,
};

static void handle_get_version(void)
{
  __write_byte(ACK);                               
  __write((uint8_t *) &get_version_return_pkt, 
          sizeof(get_version_return_pkt));    
  __write_byte(ACK);                               
}

/*
 * Read Protection Status command.
 *
 * The Read Protection Status command is reserved for the future.
 */

static void handle_read_protection_status(void)
{

}

/*
 * The Get ID command.
 *
 * The Get ID command is used to get the version of the chip ID (identification).
 */

static const get_id_return_pkt_t get_id_return_pkt = 
{
  .N = 1,                         /* Number of bytes to be followed - 1. */
  .PID = CERBERUS_KEYCORE_PID,    /* Chip ID (identification). */
};

static void handle_get_id(void) 
{
  __write_byte(ACK);                            
  __write((uint8_t *) &get_id_return_pkt, sizeof(get_id_return_pkt));    
  __write_byte(ACK);                           
}

/*
 * Read Device Public Key.
 *
 * The Read Device Public Key command is used to read the device public key 
 * from the Keycore.
 */

static void handle_read_device_pk(void)
{
  __write_byte(ACK);                               
  __write_byte(DEV_PUBKEY_SIZE - 1);     
  __write((uint8_t *) dev_pub_key, sizeof(dev_pub_key));
  __write_byte(ACK);                                
}

/*
 * Read Hash Code command.
 *
 * The Read Hash Code command is used to read the application hash code (see Secure Boot
 * documentation for more detail).
 *
 * note: Read Hash Code command is valid, only if the Generate command was executed.
 */

static void handle_read_hashcode(void)
{
  if (!keycore_hashcode_isvalid()) { /* The Hashcode is not valid */
    /* Sends a NACK byte and end of command. */
    __write_byte(NACK);        
    return;
  } 
  else {
    /* Else, send an ACK byte.
     * The command shall be executed. */
    __write_byte(ACK);
  } 

  __write_byte(sizeof(sec_hash) - 1);               /* Send the length of hash code bytes. */
  __write((uint8_t *) sec_hash, sizeof(sec_hash));  /* Send Hash code */ 
  
  /* Send an ACK bytes 
   * and end of Read Hash Code command. */
  __write_byte(ACK);                                    
}

/*
 * Read Security Monitor Public Key.
 *
 * The Read Security Monitor Public Key command is used for reading the Security 
 * Monitor Public Key (see Secure Boot documentation for more detail).
 *
 * note: Read Security Monitor Public Key (or Read SM Public Key) command is valid, only if 
 *       the Generate command was executed.
 */
 
static void handle_read_sec_pk(void)
{
    if (!keycore_seckeys_isvalid()) { 
      /* The Security Monitor Public Key is not valid. */
      /* Send a NACK byte and end of Read Security Monitor Public Key. */
      __write_byte(NACK);
      return;
    } 
    else {
      /* Else, send an ACK byte. 
       * The command shall be executed.*/
      __write_byte(ACK);
    }

    __write_byte(sizeof(sec_pub_key) - 1);   /* Send the length of security monitor PK. */
    __write((uint8_t *) sec_pub_key,         /* Send Security Monitor PK. */
            sizeof(sec_pub_key));
    
    /* Send an ACK byte and 
     * end of Read SM Public Key. */
    __write_byte(ACK);                                    
}

/*
 * Read Security Monitor Signature.
 *
 * The Read Security Monitor Signature command is used for reading the Security
 * Monitor Signature (see Scure Boot documentation for more detail).
 *
 * note: Read Security Monitor Signature (or Read SM Public Key) command is valid, only if 
 *       the Generate command was executed.
 */
static void handle_read_sec_signature(void)
{
    if (!keycore_seckeys_isvalid()) {
        /* The Security Monitor Public Key is not valid. */
        /* Send a NACK byte and end of Read Security Monitor Signature. */
        __write_byte(NACK);
        return;
    }
    else {
        /* Else, send an ACK byte. 
         * The command shall be executed.*/
        __write_byte(ACK);
    }
    
    __write_byte(sizeof(sec_signa) - 1);   /* Send the length of security monitor PK. */
    __write((uint8_t *) sec_signa, sizeof(sec_signa));  /* Send Security Monitor PK. */
    
    /* Send an ACK byte and 
     * end of Read SM Public Key. */
    __write_byte(ACK);             
}
 

/*
 * Generate command.
 *
 * The Generate command is used to generate the Hash Code (see in Read Hash Code command),
 * SM Public Key (see in Read SM Public Key command),...
 *
 * note: In Generate proccess, the command requires a application payload.
 */

/* This buffer is used for Generate command and Sign command. */
static uint8_t buffer[256];     

static void handle_generate(void)
{
  /* Send an ACK byte and 
   * start of generate process. */
  __write_byte(ACK);
  
  /* Receives the length of payload bytes (4 bytes)
   * and a checksum (1 byte). */
  __read(buffer, 4 + 1 /* 4 bytes length and 1 byte checksum */);

  /* Checksum received data. */
  uint8_t selfcheck = 0x00;
  for (int i = 0; i < 4; i++) 
    selfcheck ^= buffer[i];
  
  if (selfcheck != buffer[4]) {
    /* Send an NACK byte 
     * and end of Generate command.*/
    __write_byte(NACK);
    return;
  }
  else {
    /* Else, Send a ACK byte.
     * The Application Payload shall be received. */
    __write_byte(ACK);
  }
    
  uint32_t payload_len = GET32(buffer[0], buffer[1],  \
              buffer[2], buffer[3]);

  /* Number of data received bytes 
   * (not included: segment N, type, checksum). */
  uint32_t received_bytes_counter = 0;  
  
  keycore_payload_init();
  
  while (received_bytes_counter < payload_len) {
    uint8_t segment_len = __read_byte();    /* Receives the length of segment. */
    __read(buffer, segment_len + 1);   /* Receives segment bytes. */
    
    /* Checksum received data. */
    selfcheck = segment_len;
    for (int i = 0; i < segment_len; i++) {
      selfcheck ^= buffer[i];
    }
    
    if (selfcheck != buffer[segment_len]) { /* Checksum is not correct. */
      /* Send an NACK byte 
       * and end of Generate command. */
      __write_byte(NACK);
      return;
    }
       
    keycore_data_segment_t *seg = (keycore_data_segment_t *) buffer;
    keycore_payload_append(seg->data, segment_len - 1);
    received_bytes_counter += segment_len - 1;

    if (seg->type == KEYCORE_SEG_START || 
      seg->type == KEYCORE_SEG_NEXT) {
      // Nothing.
    }
    else if (seg->type == KEYCORE_SEG_LAST) {
      __write_byte(ACK);
      break;
    }
    else {
      __write_byte(NACK);
      return;
    }
    
    __write_byte(ACK);
  }
    
  if (received_bytes_counter != payload_len) { 
    /* The size of payload is not correct. */
    __write_byte(NACK);
    return;
  }
  
  keycore_payload_final();
  
  /* Send an ACK and
   * end of Generate command. */
  __write_byte(ACK);
  
  msg_debug("Sercurity Monitor Hash Code:\r\n");
  print_hex(sec_hash, sizeof(sec_hash));
  msg_debug("Sercurity Monitor Public Key:\r\n");
  print_hex(sec_pub_key, sizeof(sec_pub_key));
  msg_debug("Security Monitor Signature: \r\n");
  print_hex(sec_signa, sizeof(sec_signa));
}

/*
 * The Sign command is used to create a signature for messgae (see Secure Boot 
 * documentation for more detail). The Security Monitor Public and Private Key 
 * are used for generating a signature in this command.
 */

static void handle_sec_sign(void)
{
  if (!keycore_seckeys_isvalid()) { 
    /* The Security Monitor Public and Private Key is not valid */
    __write_byte(NACK);
    return;
  }
  else {
    /* Else, Send an ACK byte. */
    /* The command shall be received. */
    __write_byte(ACK);
  }

  /* Waits for receiving the size of message (4 byte) and
   * checksum (1 byte).*/
  __read(buffer, 4 + 1);

  uint8_t selfcheck = 0x00;
  for (int i = 0; i < 4; i++) {
    selfcheck ^= buffer[i];
  }

  if (selfcheck != buffer[4]) { /* Checksum is not correct.*/ 
    __write_byte(NACK);
    return;
  }
  else {
    /* Else, send an ACK byte. 
     * The message shall be received. */
    __write_byte(ACK);
  }

  uint32_t msg_len = GET32(buffer[0], buffer[1], buffer[2], buffer[3]);

  /* Receives payload */
  uint32_t received_bytes_counter = 0;

  keycore_signature_msg_init();

  while (received_bytes_counter < msg_len) {
    uint8_t segment_len = __read_byte();      /* Receives the length of segment. */
    __read(buffer, segment_len + 1);     /* Receives segment bytes. */

    /* Checksum received data. */
    selfcheck = segment_len;
    for (int i = 0; i < segment_len; i++) {
        selfcheck ^= buffer[i];
    }

    if (selfcheck != buffer[segment_len]) { /* Checksum is not correct. */
      /* Send an NACK byte 
       * and end of Generate command. */
      __write_byte(NACK);
      return;
    }

    keycore_data_segment_t *seg = (keycore_data_segment_t *) buffer;
    keycore_signature_msg_append(seg->data, segment_len - 1);
    received_bytes_counter += segment_len - 1;

    if (seg->type == KEYCORE_SEG_START || 
      seg->type == KEYCORE_SEG_NEXT) {
      // Nothing.
    }
    else if (seg->type == KEYCORE_SEG_LAST) {
      __write_byte(ACK);
      break;
    }
    else {
      __write_byte(NACK);
      return;
    }

    __write_byte(ACK);	
  }

  if (received_bytes_counter != msg_len) { 
    /* The size of payload is not correct. */
    __write_byte(NACK);
    return;
  }
  else {
    __write_byte(ACK);
  }
  
  uint8_t signature[SEC_SIGNA_SIZE];
  keycore_signature_msg_final(&signature[0]);

  __write_byte(sizeof(signature) - 1);
  __write((uint8_t *) &signature[0], sizeof(signature));

  /* Send an ACK and
   * end of Generate command. */
  __write_byte(ACK);
}

/*******************************************************************************
 * API
 ******************************************************************************/

void keycore_cmd_handler(void)
{
    uint8_t code;
    uint8_t checksum;
    const uint8_t seq_max = 4;
    uint8_t seq[seq_max];
    
    while (1)
    {
        __read(buffer, 1);
        for (int i = 1; i < seq_max; i++)  {
            seq[i-1] = seq[i];
        }   
        seq[3] = buffer[0];
        
        for (int i = 0; i < seq_max-1; i++) {
            uint8_t checksum = ~seq[i+1];
            if (seq[i] == checksum) {
                code = seq[i];
                seq[i] = 0x00;
                seq[i+1] = 0x00;
                goto handler;
            }
        }
        
        continue;
        
handler:       
        switch (code) {
        case KEYCORE_CMD_GET:
            msg_info("Get command is run\r\n");
            handle_get();
            break;
        case KEYCORE_CMD_GET_VERSION:
            msg_info("Get version is run");
            handle_get_version();
            break;
        case KEYCORE_CMD_READ_PRO_STATUS:
            msg_info("Read protection status command is run\r\n");
            handle_read_protection_status();
            break;
        case KEYCORE_CMD_GET_ID:
            msg_info("Get id command is run\r\n");
            handle_get_id();
            break;
        case KEYCORE_CMD_READ_DEVICE_PK:
            msg_info("Read manufacture public key is run\r\n");
            handle_read_device_pk();
            break;
        case KEYCORE_CMD_READ_SEC_HASHCODE:
            msg_info("Read secuirity monitor hash code command is run\r\n");
            handle_read_hashcode();
            break;
        case KEYCORE_CMD_READ_SEC_PK:
            msg_info("Read secuirity monitor public key command is run\r\n");
            handle_read_sec_pk();
            break;
        case KEYCORE_CMD_READ_SEC_SIGNATURE:
            msg_info("Read secuirity monitor signature command is run\r\n");
            handle_read_sec_signature();
            break;
        case KEYCORE_CMD_GENERATE:
            msg_info("Build command is run\r\n");
            handle_generate();
            break;
        case KEYCORE_CMD_SEC_SIGN:
            msg_info("Sign command is run\r\n");
            handle_sec_sign();
            break;
        default:
            __write_byte(NACK);
        }
    }
}















