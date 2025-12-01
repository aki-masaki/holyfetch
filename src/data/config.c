#include "data/config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cfg_extract_value(char *line, char *field, char *value) {
  char *eqi = strchr(line, '=');
  if (!eqi)
    return -1;

  size_t len = eqi - line;
  if (len >= 64)
    len = 63;

  strncpy(field, line, len);
  field[len] = '\0';

  snprintf(value, 128, "%s", eqi + 1);

  return 0;
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
      if (strncmp(line + 1, "line", 4) == 0) {
        // 012345. <- line[6] is here
        // @line(i)
        int i = line[6] - '0'; // in the ascii table, '0' is n distance from any number ('4' - '0' = 4)

        if (fgets(line, 256, file))
          snprintf(out->lines[i], 128, "%s", line);

        out->line_cnt++;
      }
    } else if (cfg_extract_value(line, field, value) == 0) {

    }
  }
}
