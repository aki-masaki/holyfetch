#include "collectors/os.h"
#include "collectors/general.h"
#include "config.h"

int main() {
  os_info os_info;
  get_os_info(&os_info);

  config config;
  read_config(&config);

  return 0;
}
