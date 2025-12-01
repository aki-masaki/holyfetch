#include "data/config.h"
#include "data/fetch.h"
#include "display/template.h"
#include "display/colors.h"
#include "display/display.h"

#include <stdio.h>

int main() {
  char error[128] = {0};

  config config;
  if (read_config(&config, error) == -1) {
    printf(RED "%s\n" RESET, error);

    return 0;
  }

  fetch_data fetch_data;
  perform_fetch(&fetch_data);

  char lines[10][1024];

  for (int i = 0; i < 10; i++) {
    if (config.line_def[i] == 0) continue;

    if (expand_template(config.lines[i], lines[i], 1024, fetch_data, error, config, 0) == -1) {
      printf(RED "%s\n" RESET, error);

      return 0;
    }
  }

  display(lines, config);

  return 0;
}
