typedef struct {
  char os_name[64];
  char kernel_release[64];
  char machine[16];
} os_info;

void get_os_info(os_info *out);
