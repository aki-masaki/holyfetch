#include "data/config.h"
#include "display/colors.h"

#include <ctype.h>
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

int read_config(config *out, char *error) {
  out->line_cnt = 0;

  for (int i = 0; i < 10; i++)
    out->line_def[i] = 0;

  const char *home = getenv("HOME");
  if (!home) {
    sprintf(error, "%s", "can't get HOME env");

    return -1;
  }

  char path[256] = {0};
  snprintf(path, sizeof(path), "%s/.holyfetch", home);

  FILE *file = fopen(path, "r");

  if (file == NULL) {
    sprintf(error, "%s", "config file doens't exist");

    return -1;
  }

  char line[256] = {0};
  char field[64] = {0};
  char value[256] = {0};

  while (fgets(line, 256, file)) {
    if (line[0] == '@') {
      if (strncmp(line + 1, "line", 4) == 0) { // +1 for \n
        // 012345. <- line[6] is here
        // @line(i)
        int i = line[6] - '0'; // in the ascii table, '0' is n distance from any
                               // number ('4' - '0' = 4)

        if (!isdigit(line[6])) {
          sprintf(error, "line number %c not numeric", line[6]);

          return -1;
        }

        if (fgets(line, 256, file)) {
          if (line[0] == '@') {
            sprintf(error, "line %d not defined", i);

            return -1;
          }

          sprintf(out->lines[i], "%s", line);
        }

        out->line_cnt++;
        out->line_def[i] = 1;
      }
    } else if (cfg_extract_value(line, field, value) == 0) {
      value[strlen(value) - 1] = '\0';

      if (strcmp(field, "art_color") == 0) {
        const char *color = NULL;

        for (size_t k = 0; k < colors_count; k++) {
          if (strncmp(value, colors[k].key, strlen(colors[k].key)) == 0) {
            color = colors[k].value;
            break;
          }
        }

        if (color)
          snprintf(out->art_color, 16, "%s", color);
        else {
          snprintf(error, 128, "color %s doesn't exist", value);

          return -1;
        }
      }
    }
  }

  return 0;
}
