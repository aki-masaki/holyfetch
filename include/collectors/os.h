typedef struct {
  char name[64];
  char pretty_name[64];
  char id[16];
  char build_id[32];

  struct {
    char name[64];
    char release[64];
    char machine[16];
  } kernel;
} os_info;

void get_os_info(os_info *out);
