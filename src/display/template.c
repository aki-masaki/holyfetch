#include "display/template.h"
#include "display/colors.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int handle_colors(char *line, char *out, size_t out_len, char *error) {
  for (int i = 0; line[i]; i++) {
    // start of color
    if (line[i] == '#') {
      // pointer to the next '<' after '#'
      // #blue<...>
      //      - here
      char *lb = strchr(line + i, '<');

      if (lb == NULL) {
        sprintf(error, "missing '<' after color declaration at col:%d", i);

        return -1;
      }

      // how many characters are from '<' to the character after '#' ? (in memory)
      // #blue<...>
      // 012345 5 - 0 - 1 = 4 (blue) 
      size_t clrlen = lb - (line + i) - 1;

      if (clrlen > 7) {
        strcpy(error, "color can't be more than 7 characters");

        return -1;
      }

      // longest allowed color key is 'magenta' (7 characters + 1 for \0)
      char color_key[8];

      // line + i + 1 is the character after '#' (the first character of the color key)
      // so copy from there, clrlen characters (the amount of characters to '<')
      // since clrlen is maximum 7, we dont have to worry about buffer overflows
      sprintf(color_key, "%.*s", (int)clrlen, line + i + 1);

      char content[128];
      
      // pointer to the next '>' after '<'
      // #blue<...>
      //          - here
      char *rb = strchr(lb, '>');

      if (rb == NULL) {
        sprintf(error, "missing '>' after color %s", color_key);

        return -1;
      }

      // how many characters are from '>' to the character after '<' ? (in memory)
      // #blue<...>
      //      01234 4 - 0 - 1 = 3 (...)
      size_t cntlen = rb - lb - 1;

      if (cntlen > 127) {
        sprintf(error, "color %s can't contain more than 127 characters", color_key);

        return -1;
      }

      // lb + 1 is the character after '<' (the first character of content)
      // so copy from there, cntlen characters (the amount of characters to '>')
      // since cntlen is maximum 127, we dont have to worry about buffer overflows
      sprintf(content, "%.*s", (int)cntlen, lb + 1);

      // we have the color_key but not the actual ASCII code
      const char *color_code = NULL;

      for (int k = 0; k < (int)colors_count; k++) {
        if (strcmp(color_key, colors[k].key) == 0) {
          color_code = colors[k].value;
          break;
        }
      }

      if (color_code == NULL) {
        sprintf(error, "color %s doesn't exist", color_key);

        return -1;
      }

      // at this point we have both the code for the color and the content
      // all it's left is to add them to out
      
      // current length of out
      size_t outclen = strlen(out);
      
      // the maximum size they both occupy is 6 bytes (color_code) + 128 (content) + 6 (reset color code) = 140
      if (outclen + 140 >= out_len) {
        sprintf(error, "buffer overflow from color %s", color_key);

        return -1;
      }

      // concatenate each to out
      strcat(out, color_code);
      strcat(out, content);
      strcat(out, RESET);

      // move i to the index one character after '>'
      i = rb - line;
    } else {
      // if its not a color, just add it to out
      size_t out_len_current = strlen(out);

      if (out_len_current + 1 < out_len) {
        out[out_len_current] = line[i];
        out[out_len_current + 1] = '\0';
      }
    }
  }

  printf("%s\n", out);

  return 0;
}

int expand_template(char *line, char *out, size_t out_len, fetch_data data,
                    char *error, config config, int inner) {
  if (handle_colors(line, out, out_len, error) != 0)
    return -1;

  return 0;
}
