#include "collectors/os.h"

typedef struct {
  os_info os;
} fetch_data;

void perform_fetch(fetch_data *out);
