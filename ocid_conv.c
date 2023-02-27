/*
 * Copyright (C) 2023
 *
 * Authors: marcin
 *	        biktorgj
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 */

#include "stdint.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

enum ocid_radio_type {
  OCID_RADIO_GSM = 0x01,
  OCID_RADIO_CDMA = 0x02,
  OCID_RADIO_UMTS = 0x05,
  OCID_RADIO_LTE = 0x0b,
  OCID_RADIO_NR = 0x0e,
} __attribute__((__packed__));

struct ocid_cell {
  uint8_t radio;
  uint16_t mcc;
  uint16_t net;
  uint32_t area;
  uint64_t cell;
  uint16_t unit;
  float lon;
  float lat;
  uint32_t range;
  uint32_t samples;
  uint8_t changeable;
  uint32_t created;
  uint32_t updated;
  int16_t average_signal;
};

struct ocid_cell_small {
  uint8_t radio;
  uint32_t area;
  uint32_t cell;
  float lon;
  float lat;
  uint32_t range;
  uint32_t updated;
  int16_t average_signal;
} __attribute__ ((__packed__));

int main(int argc, char **argv) {
  char buf[8192], *sbuf = NULL;
  FILE *fp = NULL;
  FILE *fout = NULL;
  if (argc < 2) {
    fprintf(stderr, "You must include the path to the cell towers csv file!\n");
    return -ENOENT;
  }
  fp = fopen(argv[1], "r");
  if (fp == NULL) {
    fprintf(stderr, "Error opening '%s'\n", argv[1]);
    return -ENOENT;
  }

  if (fgets(buf, 8192, fp) != NULL) {
     if (strcmp("radio,mcc,net,area,cell,unit,lon,lat,range,samples,changeable,"
             "created,updated,averageSignal\n",
             buf)) {
    fprintf(stderr, "%s: Invalid file format\n", __func__);
    return -EINVAL;
  }
  }
  fprintf(stdout, "Processing data, please stand by...\n");
  while (fgets(buf, 8192, fp) != NULL) {
    struct ocid_cell *cell = calloc(1, sizeof(struct ocid_cell));
    struct ocid_cell_small *smcell = calloc(1, sizeof(struct ocid_cell_small));
    char curr_file_path[256] = {0};
    switch (buf[0]) {
    case 'G':
      cell->radio = OCID_RADIO_GSM;
      sbuf = buf + 4;
      break;
    case 'C':
      cell->radio = OCID_RADIO_CDMA;
      sbuf = buf + 5;
      break;
    case 'U':
      cell->radio = OCID_RADIO_UMTS;
      sbuf = buf + 5;
      break;
    case 'L':
      cell->radio = OCID_RADIO_LTE;
      sbuf = buf + 4;
      break;
    case 'N':
      cell->radio = OCID_RADIO_NR;
      sbuf = buf + 3;
      break;
    }

    sscanf(sbuf, "%hu,%hu,%u,%lu,%hu,%g,%g,%u,%u,%hhu,%u,%u,%hd", &cell->mcc,
           &cell->net, &cell->area, &cell->cell, &cell->unit, &cell->lon,
           &cell->lat, &cell->range, &cell->samples, &cell->changeable,
           &cell->created, &cell->updated, &cell->average_signal);

    snprintf(curr_file_path, 255, "./out/%u-%.2u.bin", cell->mcc, cell->net);
  /* Flooding stdout with all the table helps nobody */
  /*  printf("%hu\t%hu\t%u\t%lu\t%hu\t%g\t%g\t%u\t%u\t%hhu\t%u\t%u\t%hd\t%s\n",cell->mcc,
           cell->net, cell->area, cell->cell, cell->unit, cell->lon,
           cell->lat, cell->range, cell->samples, cell->changeable,
           cell->created, cell->updated, cell->average_signal, curr_file_path);
*/
    fout = fopen(curr_file_path, "a");
    if (fout == NULL) {
      fprintf(stderr, "Error writing to %s\n", curr_file_path);
      return -EINVAL;
    }
    smcell->radio = cell->radio;
    smcell->area = cell->area;
    smcell->range = cell->range;
    smcell->cell = cell->cell;
    smcell->lat = cell->lat;
    smcell->lon = cell->lon;
    smcell->average_signal = cell->average_signal;
    smcell->updated = cell->updated;

    fseek(fout, 0, SEEK_END);
    fwrite(smcell, sizeof(struct ocid_cell_small), 1, fout);
    fclose(fout);
    free(cell);
    free(smcell);
  }
  return 0;
}

