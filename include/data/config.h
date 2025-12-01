#pragma once

typedef struct {
  char message[128];
  char lines[3][256];
  int line_cnt;
} config;

void read_config(config *out);
