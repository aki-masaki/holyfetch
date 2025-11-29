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
    strcpy(out->name, "linux");

    return;
  }

  char line[256];

  while (fgets(line, 256, file)) {
    if (strncmp(line, "NAME", 4) == 0)
      extract_value(out->name, line, sizeof(out->name));
    else if (strncmp(line, "PRETTY", 6) == 0)
      extract_value(out->pretty_name, line, sizeof(out->pretty_name));
    else if (strncmp(line, "ID", 2) == 0)
      extract_value(out->id, line, sizeof(out->id));
    else if (strncmp(line, "BUILD_ID", 7) == 0)
      extract_value(out->build_id, line, sizeof(out->build_id));
  }

  fclose(file);
}

void get_kernel_data(os_info *out) {
  struct utsname uname_data;
  uname(&uname_data);

  strncpy(out->kernel.name, uname_data.sysname, 63);
  out->kernel.name[63] = '\0';

  strncpy(out->kernel.release, uname_data.release, 63);
  out->kernel.release[63] = '\0';

  strncpy(out->kernel.machine, uname_data.machine, 15);
  out->kernel.machine[15] = '\0';
}

void get_os_info(os_info *out) {
  get_os_name(out);
  get_kernel_data(out);
}
