#pragma once

#include <stdlib.h>

#include "data/fetch.h"
#include "data/config.h"

int expand_template(char *line, char *out, size_t out_len, fetch_data data, char *error, config config);
