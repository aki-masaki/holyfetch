#include "general.h"
#include <unistd.h>
#include <string.h>

void get_username(char *out) {
  strncpy(out, getlogin(), 31);
  out[31] = '\0';
}

void get_hostname(char *out) {
  gethostname(out, 63);
  out[63] = '\0';
}
