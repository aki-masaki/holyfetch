#include <stdio.h>
#include "collectors/os.h"

int main() {
  os_info os_info;
  get_os_info(&os_info);

  printf("%s", os_info.kernel.name);

  return 0;
}
