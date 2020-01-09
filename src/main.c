#include <stdio.h>
#include <stdlib.h>
#include "serial.h"
#include "cmd_handler.h"

static const char* portname;

void System_init(void)
{
    serial_init(portname);
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        printf("Syntac: %s portname\n", argv[0]);
        exit(-1);
    }

    portname = argv[1];

    System_init();
    
    while (1) {
        keycore_cmd_handler();
    }
}
