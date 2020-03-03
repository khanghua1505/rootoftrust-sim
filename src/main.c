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
#include <stdlib.h>
#include "keycore.h"
#include "cmd_handler.h"
#include "config.h"
#include "utils.h"

#ifdef CONFIG_INTERFACE_SERIAL
#  include "serial.h"
#elif defined(CONFIG_INTERFACE_ETHERNET)
#  include "ethernet.h"
#endif

void log_write(const uint8_t *buffer, size_t len)
{
  fwrite(buffer, len, 1, stdout);
}

static void print_key(const uint8_t *pk, uint16_t key_size)
{
  for (int i = 0; i < key_size; i += 8) {
    for (int j = 0; j < 8; j++) {
      printf("%2x ", pk[i]);
    }
    printf("\r\n");
  }
}

int main(int argc, char **argv)
{
  FILE *fd;
  const char *key_file;
  
  log_io_set(log_write);
  
#ifdef CONFIG_INTERFACE_SERIAL
  if (argc != 3) {
    printf("Syntax Error: %s PORT KEY_FILE\n", argv[0]);
    exit(-1);
  }
  
  serial_init(argv[1]);
  key_file = argv[2];
  
  msg_info("Interface: Serial Info\r\n");
  msg_info("Port: %s Baudrate: 115200\r\n", argv[1]);
#elif defined(CONFIG_INTERFACE_ETHERNET) 
  if (argc != 2) {
    printf("Syntax Error: %s KEY_FILE\n", argv[0]);
    exit(-1);
  }
  
  msg_info("Interface: Ethernet\r\n");
  if (socket_open() < 0) {
    exit(-1);
  }
  
  key_file = argv[1];
#endif

  fd = fopen(key_file, "rb");
  if (fd == NULL) {
    msg_error("Can't open %s file.\r\n", key_file);
    exit(-1);
  }
  
  fread(dev_pub_key, sizeof(dev_pub_key), 1, fd);
  fread(dev_prv_key, sizeof(dev_prv_key), 1, fd);
  
  printf("Device Public Key: \r\n");
  print_key(dev_pub_key, sizeof(dev_pub_key));
  
  keycore_cmd_handler();
  
  return 0;
    
}
