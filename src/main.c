#include "config.h"
#include "fetch.h"
#include "template.h"
#include "colors.h"

#include <stdio.h>

int main() {
  config config;
  read_config(&config);

  fetch_data fetch_data;
  perform_fetch(&fetch_data);

  char output[2048];
  char error[128];

  if (expand_template(config.output, output, 2048, fetch_data, error) == 0)
    printf("%s", output);
  else
    printf(RED "error" RESET ": %s\n", error);

  return 0;
}
