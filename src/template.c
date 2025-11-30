#include "template.h"
#include "colors.h"

#include <stdio.h>
#include <string.h>

color_map_t colors[] = {
    {"red", RED},         {"green", GREEN}, {"yellow", YELLOW}, {"blue", BLUE},
    {"magenta", MAGENTA}, {"cyan", CYAN},   {"nocolor", RESET},
};

size_t colors_count = sizeof(colors) / sizeof(colors[0]);

int handle_color(char *line, int *i, char *out, int out_len, char *error) {
  char *end = strchr(line + *i, ']');

  char color[16];

  int len = end - (line + *i);

  if (len >= 16)
    len = 15;

  snprintf(color, len + 1, "%.*s", len - 1, line + *i + 1);

  const char *val = NULL;

  for (size_t i = 0; i < colors_count; i++) {
    if (strcmp(color, colors[i].key) == 0) {
      val = colors[i].value;
      break;
    }
  }

  if (val)
    strncat(out, val, out_len - strlen(out) - 1);
  else {
    snprintf(error, 128, "color %s doesn't exist", color);
    return -1;
  }

  return 0;
}

int handle_value(char *line, int *i, char *out, int out_len, char *error, fetch_data data) {
  char *end = strchr(line + *i, '}');

  char key[16];

  int len = end - (line + *i);

  if (len >= 16)
    len = 15;

  snprintf(key, len + 1, "%.*s", len - 1, line + *i + 1);

  if (strcmp(key, "pretty_name") == 0)
    strncat(out, data.os.pretty_name, out_len - strlen(out) - 1);
  else if (strcmp(key, "name") == 0)
    strncat(out, data.os.name, out_len - strlen(out) - 1);
  else if (strcmp(key, "id") == 0)
    strncat(out, data.os.id, out_len - strlen(out) - 1);
  else if (strcmp(key, "build_id") == 0)
    strncat(out, data.os.build_id, out_len - strlen(out) - 1);
  else if (strcmp(key, "kernel_name") == 0)
    strncat(out, data.os.kernel.name, out_len - strlen(out) - 1);
  else if (strcmp(key, "kernel_machine") == 0)
    strncat(out, data.os.kernel.machine, out_len - strlen(out) - 1);
  else if (strcmp(key, "kernel_release") == 0)
    strncat(out, data.os.kernel.release, out_len - strlen(out) - 1);
  else if (strcmp(key, "username") == 0)
    strncat(out, data.username, out_len - strlen(out) - 1);
  else if (strcmp(key, "hostname") == 0)
    strncat(out, data.hostname, out_len - strlen(out) - 1);
  else {
    snprintf(error, 128, "key %s doesn't exist", key);

    return -1;
  }

  return 0;
}

int expand_template(char *template, char *out, size_t out_len, fetch_data data,
                    char *error) {
  char *src = template;
  out[0] = '\0';
  char *line = strtok(src, "\n");

  while (line != NULL) {
    for (int i = 0; line[i]; i++) {
      if (line[i] == '[' && i + 1 < (int)strlen(line) && line[i + 1] != '[') {
        if (handle_color(line, &i, out, out_len, error) != 0)
          return -1;
      }
      else if (line[i] == '{') {
        if (handle_value(line, &i, out, out_len, error, data) != 0)
          return -1;
      }
    }

    line = strtok(NULL, "\n");
  }

  return 0;
}
