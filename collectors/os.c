#include "os.h"
#include <stdio.h>
#include <string.h>
#include <sys/utsname.h>

void extract_value(char *dest, char *line, size_t size) {
  char *start = strchr(line, '"');
  char *end = strrchr(line, '"');

  if (start && end && end > start) {
    size_t len = end - (start + 1);

    if (len >= size)
      len = size - 2;

    memcpy(dest, start + 1, len);
    dest[len] = '\0';
  }
}

void get_os_name(os_info *out) {
  FILE *file = fopen("/etc/os-release", "r");

  if (file == NULL) {
    strcpy(out->os_name, "linux");

    return;
  }

  char line[256];

  while (fgets(line, 256, file)) {
    if (strncmp(line, "PRETTY", 6) == 0)
      extract_value(out->os_name, line, sizeof(out->os_name));
  }

  fclose(file);
}

void get_os_info(os_info *out) {
  get_os_name(out);

  struct utsname uname_data;
  uname(&uname_data);

  strncpy(out->kernel_release, uname_data.release, 63);
  out->kernel_release[63] = '\0';

  strncpy(out->machine, uname_data.machine, 15);
  out->machine[15] = '\0';
}
