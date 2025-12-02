#include "display/template.h"
#include "display/colors.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

int apply_mod(char *value, char *out, char *mod) {
  if (strcmp(mod, "lower") == 0) {
    for (int i = 0; value[i]; i++)
      out[i] = tolower(value[i]);
  } else if (strcmp(mod, "upper") == 0) {
    for (int i = 0; value[i]; i++)
      out[i] = toupper(value[i]);
  } else if (strcmp(mod, "basename") == 0) {
    char *lastslash = strrchr(value, '/');

    strcpy(out, lastslash + 1);
  } else
    return -1;

  out[strlen(value)] = '\0';

  return 0;
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

  char key[32] = {0};

  int len = end - (line + *i) - 1;

  if (len >= 32)
    len = 31;

  snprintf(key, sizeof(key), "%.*s", len, line + *i + 1);

  char value[128] = {0};

  if (get_value_by_key(data, key, value, 128) == 0) {
    *i += len + 1;

    size_t line_len = strlen(line);

    if (*i + 1 < (int)line_len && line[*i + 1] == ':') {
      // skip :
      (*i)++;

      char *rcol = strchr(line + *i + 1, ':');
      size_t modlen = rcol - (line + *i + 1);

      if (modlen >= 64)
        modlen = 64;

      char mod[64] = {0};
      snprintf(mod, sizeof(mod), "%.*s", (int)modlen, line + *i + 1);

      *i += modlen + 1;

      char final_value[128] = {0};

      if (apply_mod(value, final_value, mod) == -1) {
        snprintf(error, 128, "mod %s doesn't exist", mod);

        return -1;
      }

      strcpy(value, final_value);
    }

    strncat(out, value, out_len - strlen(out) - 1);
  } else {
    snprintf(error, 128, "key %s doesn't exist", key);

    return -1;
  }

  return 0;
}

int handle_env(char *line, int *i, char *out, int out_len, char *error) {
  char *end = strchr(line + *i + 1, '$');

  char key[64] = {0};

  int len = end - (line + *i) - 1;

  if (len >= 64)
    len = 63;

  snprintf(key, sizeof(key), "%.*s", len, line + *i + 1);

  char *env = getenv(key);

  if (env) {
    *i += len + 1;

    size_t line_len = strlen(line);

    if (*i + 1 < (int)line_len && line[*i + 1] == ':') {
      // skip :
      (*i)++;

      char *rcol = strchr(line + *i + 1, ':');
      size_t modlen = rcol - (line + *i + 1);

      if (modlen >= 64)
        modlen = 64;

      char mod[64] = {0};
      snprintf(mod, sizeof(mod), "%.*s", (int)modlen, line + *i + 1);

      *i += modlen + 1;

      char final_value[128] = {0};

      if (apply_mod((char *)env, final_value, mod) == -1) {
        snprintf(error, 128, "mod %s doesn't exist", mod);

        return -1;
      }

      strcpy(env, final_value);
    }

    strncat(out, env, out_len - strlen(out) - 1);
  } else {
    snprintf(error, 128, "can't get env %s", key);

    return -1;
  }

  return 0;
}

int expand_template(char *line, char *out, size_t out_len, fetch_data data,
                    char *error, config config, int inner) {
  char ch = '\0';
  out[0] = '\0';

  int line_len = strlen(line);

  for (int i = 0; i < line_len; i++) {
    ch = line[i];

    // color
    if (ch == '#' && !inner) {
      if (i + 1 < line_len && line[i + 1] == '#') {
        size_t out_len_current = strlen(out);

        if (out_len_current + 1 < out_len) {
          out[out_len_current] = '#';
          out[out_len_current + 1] = '\0';
        }

        i++;
        continue;
      }

      // find the pointer to the start of (
      // blue(
      //     ^                     + 1 for #
      char *lpar = strchr(line + i + 1, '(');

      if (!lpar) {
        snprintf(error, 128, "missing '(' after color");

        return -1;
      }

      size_t len = lpar - line - i - 1;
      if (len >= 64)
        len = 63;

      char color[64] = {0};
      snprintf(color, len + 1, "%.*s", (int)len, line + i + 1);

      const char *val = NULL;

      for (size_t k = 0; k < colors_count; k++) {
        if (strcmp(color, colors[k].key) == 0) {
          val = colors[k].value;
          break;
        }
      }

      if (val) {
        size_t out_len_cur = strlen(out);
        size_t val_len = strlen(val);

        if (out_len_cur + val_len >= out_len) {
          snprintf(error, 128, "buffer overflow from color %s", color);
          return -1;
        }

        strncat(out, val, out_len - out_len_cur - 1);
      } else {
        snprintf(error, 128, "color %s doesn't exist", color);

        return -1;
      }

      // find the pointer to the ) from ( + 1
      // blue(os)
      //     -  ^
      char *rpar = strchr(lpar + 1, ')');

      // if the char before it is '\', it is escaped
      while (rpar && (rpar - 1)[0] == '\\')
        rpar = strchr(rpar + 1, ')');

      if (!rpar) {
        snprintf(error, 128, "missing closing ')' from color %s", color);

        return -1;
      }

      size_t cnt_len = rpar - lpar - 1;

      if (cnt_len >= 128)
        cnt_len = 128;

      char content[128] = {0};
      snprintf(content, cnt_len + 1, "%.*s", (int)cnt_len, lpar + 1);

      char final_content[256] = {0};
      expand_template(content, final_content, 256, data, error, config, 1);

      strncat(out, final_content, out_len - strlen(out) - 1);

      // reset color
      strncat(out, RESET, out_len - strlen(out) - 1);

      i = (rpar - line);
    } else if (ch == '.') {
      if (i + 1 < line_len && line[i + 1] == '.') {
        size_t out_len_current = strlen(out);

        if (out_len_current + 1 < out_len) {
          out[out_len_current] = '.';
          out[out_len_current + 1] = '\0';
        }

        i++;
        continue;
      }

      // property
      if (handle_value(line, &i, out, out_len, error, data) == -1)
        return -1;
    } else if (ch == '$') {
      if (i + 1 < line_len && line[i + 1] == '$') {
        size_t out_len_current = strlen(out);

        if (out_len_current + 1 < out_len) {
          out[out_len_current] = '$';
          out[out_len_current + 1] = '\0';
        }

        i++;
        continue;
      }

      if (handle_env(line, &i, out, out_len, error) == -1)
        return -1;
    } else {
      if (line[i] == '\\' && i + 1 < line_len && line[i + 1] == ')')
        continue;

      size_t out_len_current = strlen(out);

      if (out_len_current + 1 < out_len) {
        out[out_len_current] = line[i];
        out[out_len_current + 1] = '\0';
      }
    }
  }

  return 0;
}
