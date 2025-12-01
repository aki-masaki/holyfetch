#include "data/config.h"
#include "data/fetch.h"
#include "display/template.h"
#include "display/colors.h"

#include <stdio.h>

int main() {
  config config;
  read_config(&config);

  fetch_data fetch_data;
  perform_fetch(&fetch_data);

  char lines[3][1024];
  char error[128];

  if (expand_template(config.lines[0], lines[0], 1024, fetch_data, error, config) == 0) {
    printf("%s", lines[0]);
  } else {
    printf("%s", error);
  }

  return 0;
}
