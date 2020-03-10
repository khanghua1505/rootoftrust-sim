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
#include <time.h>
#include "sha3.h"
#include "ed25519.h"

static uint8_t PK[32];

static uint8_t SK[64];

static uint8_t SEED[64];

static void create_seed(uint8_t *seed, int nround)
{
  int round;
  int index;
  uint8_t tmp[64];
  sha3_ctx_t sha3;
  
  sha3_init(&sha3, 64);
  
  for (round = 0; round < nround; round++) {
    for (index = 0; index < 64; index++) {
      srand(time(NULL));
      tmp[index] = rand() % 256;
    }
    sha3_update(&sha3, tmp, 64);
  }
  
  sha3_final(seed, &sha3);
}

int main(int argc, char **argv)
{
  FILE *fp;
  
  fp = fopen("demo.key", "wb");
  
  create_seed(SEED, 32);
  
  ed25519_create_keypair(PK, SK, SEED);
  
  fwrite(PK, sizeof(PK[0]), sizeof(PK)/sizeof(PK[0]), fp);
  fwrite(PK, sizeof(SK[0]), sizeof(SK)/sizeof(SK[0]), fp);
  
  fclose(fp);
  
  return 0;
}
