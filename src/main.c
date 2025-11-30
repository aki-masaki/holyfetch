#include "config.h"
#include "fetch.h"

int main() {
  config config;
  read_config(&config);

  fetch_data fetch_data;
  perform_fetch(&fetch_data);

  return 0;
}
