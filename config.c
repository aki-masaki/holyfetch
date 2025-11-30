#include "config.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int cfg_extract_value(char *line, char *field, char *value) {
  char *eqi = strchr(line, '=');
  if (!eqi) return 0;

  size_t len = eqi - line;
  if (len >= 64)
    len = 63;

  strncpy(field, line, len);
  field[len] = '\0'; 

  len = strlen(eqi + 1);
  if (len >= 128)
    len = 127;

  strncpy(value, eqi + 1, len);
  value[len] = '\0';

  return 1;
}

void read_config(config *out) {
  const char *home = getenv("HOME");
  if (!home) return;

  char path[256];
  snprintf(path, sizeof(path), "%s/.holyfetch", home);

  FILE *file = fopen(path, "r");

  if (file == NULL)
    return;

  char line[256];
  char field[64];
  char value[128];

  while (fgets(line, 256, file)) {
    if (cfg_extract_value(line, field, value)) {
      printf("%s: %s", field, value);
    }
  }
}
