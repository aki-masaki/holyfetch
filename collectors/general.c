#include "general.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void get_username(char *out) {
  strncpy(out, getlogin(), 31);
  out[31] = '\0';
}

void get_hostname(char *out) {
  gethostname(out, 63);
  out[63] = '\0';
}

// Note: this only works for pacman (arch linux)
int get_packagescount() {
  FILE *fp = popen("pacman -Q | wc -l", "r");

  char buf[32];
  fgets(buf, sizeof(buf), fp);

  int count = atoi(buf);

  pclose(fp);

  return count;
}
