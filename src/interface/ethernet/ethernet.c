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

#ifdef CONFIG_INTERFACE_ETHERNET

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "utils.h"
#include "config.h"

#define KEYCORE_PORT      8080

static int sockfd = -1;

int socket_open()
{
  int fd;
  int opt;
  int status;
  struct sockaddr_in address;

  msg_info("Socket is opening (port : %d)...\r\n", KEYCORE_PORT);
  fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    msg_error("Open socket failed\r\n");
    return -1;
  }
  msg_info("Open socket successfully\r\n");

  opt = 1;
  status = setsockopt(fd, SOL_SOCKET,
      SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
  if (status != 0) {
    msg_error("Set socket operation failed\r\n");
    return -1;
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(KEYCORE_PORT);

  msg_info("Socket is binding...\r\n");
  status = bind(fd, (struct sockaddr *) &address, sizeof(address));
  if (status < 0) {
    msg_error("Socket binds failed\r\n");
    return -1;
  }
  msg_info("Bind socket successfully\r\n");

  msg_info("Socket is listening...\r\n");
  if (listen(fd, 3) < 0) {
    msg_error("Listen failed\r\n");
    return -1;
  }

  int addrlen = sizeof(address);
  sockfd = accept(fd, (struct sockaddr *) &address, (socklen_t*)&addrlen);
  if (sockfd < 0) {
    msg_error("Socket accepts failed\r\n");
    return -1;
  }
  msg_info("Socket is accepted\r\n");

  return sockfd;
}

ssize_t socket_read(uint8_t *buffer, uint16_t len)
{
  return read(sockfd, buffer, len);
}

size_t socket_send(const uint8_t *buffer, uint16_t len)
{
  return send(sockfd, buffer, len, 0);
}

#endif  // CONFIG_INTERFACE_ETHERNET

