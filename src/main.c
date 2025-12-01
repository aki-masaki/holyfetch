#include "data/config.h"
#include "data/fetch.h"
#include "display/template.h"
#include "display/colors.h"
#include "display/display.h"

#include <stdio.h>

int main() {
  config config;
  read_config(&config);

  fetch_data fetch_data;
  perform_fetch(&fetch_data);

  char lines[10][1024];
  char error[128] = {0};

  for (int i = 0; i < 10; i++) {
    if (config.line_def[i] == 0) continue;

    if (expand_template(config.lines[i], lines[i], 1024, fetch_data, error, config) == -1) {
      printf("%s", error);

      return -1;
    }
  }

  display(lines, config);

  return 0;
}
