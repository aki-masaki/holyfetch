#pragma once

#include "collectors/os.h"
#include <sys/sysinfo.h>

typedef struct {
  os_info os;
  struct sysinfo sys;
  
  char username[32];
  char hostname[64];
  int pkgcnt;
} fetch_data;

void perform_fetch(fetch_data *out);
