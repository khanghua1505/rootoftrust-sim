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
#include <stdlib.h>
#include <stdbool.h>
#include <libserialport.h>

#define SERIAL_BAUDRATE        115200
#define SERIAL_WORDLENGTH      8
#define SERIAL_PARITY          SP_PARITY_NONE
#define SERIAL_STOPBITS        1

static struct sp_port *m_serial;

void serial_init(const char *portname)
{
    enum sp_return status;

    status = sp_get_port_by_name(portname, &m_serial);
    if (status != SP_OK) {
        printf("Error: Open %s is failed\n", portname);
        exit(status);
    }

    status = sp_open(m_serial, SP_MODE_READ_WRITE);
    if (status != SP_OK) {
        printf("Error: Open port failed");
        exit(status);
    } else {
        printf("Info: Open port %s successfully\n", portname);
    }

    status = sp_set_baudrate(m_serial, SERIAL_BAUDRATE);
    if (status != SP_OK) {
        printf("Error: Set baudrate failed");
        exit(status);
    }

    status = sp_set_bits(m_serial, SERIAL_WORDLENGTH);
    if (status != SP_OK) {
        printf("Error: Set word length failed");
        exit(status);
    }

    status = sp_set_parity(m_serial, SERIAL_PARITY);
    if (status != SP_OK) {
        printf("Error: Set parity failed");
        exit(status);
    }

    status = sp_set_stopbits(m_serial, SERIAL_STOPBITS);
    if (status != SP_OK) {
        printf("Error: Set stopbits failed");
        exit(status);
    }

    status = sp_set_flowcontrol(m_serial, SP_FLOWCONTROL_NONE );
    if (status != SP_OK) {
        printf("Error: Set flow control failed");
        exit(status);
    }
}

uint16_t kgetc(void)
{
    unsigned char c;

    sp_blocking_read(m_serial, &c, 1, 1000);

    return c;
}

void kputc(uint8_t c)
{
    sp_blocking_write(m_serial, &c, 1, 1000);
}