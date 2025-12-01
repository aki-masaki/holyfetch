#pragma once

typedef struct {
  char lines[10][256];
  int line_cnt;
  // 1 if line i is defined, 0 otherwise
  int line_def[10];
} config;

int read_config(config *out, char *error);
