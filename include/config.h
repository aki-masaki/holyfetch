typedef struct {
  char message[128];
  char output[2048];
} config;

void read_config(config *out);
