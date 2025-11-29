#include <stdio.h>
#include "collectors/os.h"

int main() {
  os_info os_info;
  get_os_info(&os_info);

  printf("os: %s\nkernel release: %s\nmachine: %s\n", os_info.os_name, os_info.kernel_release, os_info.machine);

  return 0;
}
