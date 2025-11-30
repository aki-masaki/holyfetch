#pragma once

#include <stdlib.h>

#include "fetch.h"

int expand_template(char *template, char *out, size_t out_len, fetch_data data, char *error);
