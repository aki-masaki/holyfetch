#include "template.h"

#include <stdio.h>
#include <string.h>

int expand_template(char *template, char *out, size_t out_len, fetch_data data,
                    char *error) {
  out[0] = '\0';
  char *line = strtok(template, "\n");
  int is_blank_line = 0;

  while (line != NULL) {
    for (int i = 0; line[i]; i++) {
      if (line[i] == '\\' && i + 1 < (int)strlen(line) && line[i + 1] == 'n') {
        size_t out_len_current = strlen(out);

        if (out_len_current + 1 < out_len) {
          out[out_len_current] = '\n';
          out[out_len_current + 1] = '\0';
        }

        i++;
        is_blank_line = 1;
        continue;
      }
      else if (line[i] == '%') {
        if (i + 1 < (int)strlen(line) && line[i + 1] == '%') {
          size_t out_len_current = strlen(out);

          if (out_len_current + 1 < out_len) {
            out[out_len_current] = '%';
            out[out_len_current + 1] = '\0';
          }

          i++;
          continue;
        }

        char *end = strchr(line + i + 1, '%');

        char key[256];
        size_t len = end - (line + i + 1);

        if (len >= 256)
          len = 255;

        snprintf(key, len + 1, "%.*s", (int)len, line + i + 1);

        if (strcmp(key, "PRETTY_NAME") == 0)
          strncat(out, data.os.pretty_name, out_len - strlen(out) - 1);
        else if (strcmp(key, "KERNEL_RELEASE") == 0)
          strncat(out, data.os.kernel.release, out_len - strlen(out) - 1);
        else if (strcmp(key, "USERNAME") == 0)
          strncat(out, data.username, out_len - strlen(out) - 1);
        else if (strcmp(key, "HOSTNAME") == 0)
          strncat(out, data.hostname, out_len - strlen(out) - 1);
        else {
          sprintf(error, "key %s doesn't exist", key);

          return -1;
        }

        i += len + 1;
      } else {
        size_t out_len_current = strlen(out);

        if (out_len_current + 1 < out_len) {
          out[out_len_current] = line[i];
          out[out_len_current + 1] = '\0';
        }
      }
    }

    if (!is_blank_line) {
      size_t l = strlen(out);

      if (l + 1 < out_len) {
        out[l] = '\n';
        out[l + 1] = '\0';
      }
    }

    is_blank_line = 0;

    line = strtok(NULL, "\n");
  }

  return 0;
}
