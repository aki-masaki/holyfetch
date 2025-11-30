#include "../include/config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cfg_extract_value(char *line, char *field, char *value) {
  char *eqi = strchr(line, '=');
  if (!eqi)
    return 0;

  size_t len = eqi - line;
  if (len >= 64)
    len = 63;

  strncpy(field, line, len);
  field[len] = '\0';

  snprintf(value, 128, "%s", eqi + 1);

  return 1;
}

void read_config(config *out) {
  const char *home = getenv("HOME");
  if (!home)
    return;

  char path[256];
  snprintf(path, sizeof(path), "%s/.holyfetch", home);

  FILE *file = fopen(path, "r");

  if (file == NULL)
    return;

  char line[256];
  char field[64];
  char value[128];

  while (fgets(line, 256, file)) {
    if (line[0] == '@') {
      if (strncmp(line + 1, "output", 6) == 0) {
        while (fgets(line, 256, file)) {
          // end of output
          if (line[0] == '}')
            break;

          strncat(out->output, line, 256);
        }
      }
    } else if (cfg_extract_value(line, field, value)) {
      printf("%s: %s", field, value);
    }
  }
}
