#include "display/template.h"
#include "display/colors.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

color_map colors[] = {
    {"red", RED},         {"green", GREEN}, {"yellow", YELLOW}, {"blue", BLUE},
    {"magenta", MAGENTA}, {"cyan", CYAN},   {"nocolor", RESET},
};

size_t colors_count = sizeof(colors) / sizeof(colors[0]);

void apply_mod(char *value, char *out, char *mod) {
  if (strcmp(mod, "lower") == 0) {
    for (int i = 0; value[i]; i++)
      out[i] = tolower(value[i]);
  } else if (strcmp(mod, "upper") == 0) {
    for (int i = 0; value[i]; i++)
      out[i] = toupper(value[i]);
  }

  out[strlen(value)] = '\0';
}

int get_value_by_key(fetch_data data, char *key, char *out, int out_len) {
  if (strcmp(key, "pretty_name") == 0)
    snprintf(out, out_len, "%s", data.os.pretty_name);
  else if (strcmp(key, "name") == 0)
    snprintf(out, out_len, "%s", data.os.name);
  else if (strcmp(key, "id") == 0)
    snprintf(out, out_len, "%s", data.os.id);
  else if (strcmp(key, "build_id") == 0)
    snprintf(out, out_len, "%s", data.os.build_id);
  else if (strcmp(key, "kernel_name") == 0)
    snprintf(out, out_len, "%s", data.os.kernel.name);
  else if (strcmp(key, "kernel_machine") == 0)
    snprintf(out, out_len, "%s", data.os.kernel.machine);
  else if (strcmp(key, "kernel_release") == 0)
    snprintf(out, out_len, "%s", data.os.kernel.release);
  else if (strcmp(key, "username") == 0)
    snprintf(out, out_len, "%s", data.username);
  else if (strcmp(key, "hostname") == 0)
    snprintf(out, out_len, "%s", data.hostname);
  else
    return -1;

  return 0;
}

int handle_value(char *line, int *i, char *out, int out_len, char *error,
                 fetch_data data) {
  char *end = strchr(line + *i + 1, '.');

  char key[32];

  int len = end - (line + *i) - 1;

  if (len >= 32)
    len = 31;

  snprintf(key, sizeof(key), "%.*s", len, line + *i + 1);

  char value[128];

  if (get_value_by_key(data, key, value, 128) == 0) {
    *i += len + 1;

    if (line[*i + 1] == ':') {
      // skip :
      (*i)++;

      char *rcol = strchr(line + *i + 1, ':');
      size_t modlen = rcol - (line + *i + 1);

      if (modlen >= 64)
        modlen = 64;

      char mod[64];
      snprintf(mod, sizeof(mod), "%.*s", (int)modlen, line + *i + 1);

      *i += modlen + 1;

      char final_value[128];
      apply_mod(value, final_value, mod);

      strcpy(value, final_value);
    }

    strncat(out, value, out_len - strlen(out) - 1);
  } else {
    snprintf(error, 128, "key %s doesn't exist", key);

    return -1;
  }

  return 0;
}

int expand_template(char *line, char *out, size_t out_len, fetch_data data,
                    char *error, config config) {
  char ch;
  out[0] = '\0';

  // iterate over each character (until line[i] == '\0')
  for (int i = 0; (ch = line[i]); i++) {
    // color
    if (ch == '#') {
      // find the pointer to the start of (
      // blue(
      //     ^                     + 1 for #
      char *lpar = strchr(line + i + 1, '(');

      if (!lpar) {
        snprintf(error, 128, RED "error: " RESET "missing '(' after color");
        return -1;
      }

      size_t len = lpar - line - i - 1;
      if (len >= 64)
        len = 64;

      char color[64];
      snprintf(color, len + 1, "%.*s", (int)len, line + i + 1);

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

      // find the pointer to the ) from ( + 1
      // blue(os)
      //     -  ^
      char *rpar = strchr(lpar + 1, ')');
      size_t cnt_len = rpar - lpar - 1;

      if (cnt_len >= 128)
        cnt_len = 128;

      char content[128];
      snprintf(content, cnt_len + 1, "%.*s", (int)cnt_len, lpar + 1);

      char final_content[256];
      expand_template(content, final_content, 256, data, error, config);

      strncat(out, final_content, out_len - strlen(out) - 1);

      // reset color
      strncat(out, RESET, out_len - strlen(out) - 1);

      i = (rpar - line);
    } else if (ch == '.') {
      // property
      if (handle_value(line, &i, out, out_len, error, data) == -1)
        return -1;
    } else {
      size_t out_len_current = strlen(out);

      if (out_len_current + 1 < out_len) {
        out[out_len_current] = line[i];
        out[out_len_current + 1] = '\0';
      }
    }
  }

  return 0;
}
