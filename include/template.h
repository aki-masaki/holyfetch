#pragma once

#include <stdlib.h>

#include "fetch.h"

typedef struct {
  const char *key;
  const char *value;
} color_map_t;

int expand_template(char *template, char *out, size_t out_len, fetch_data data, char *error);
