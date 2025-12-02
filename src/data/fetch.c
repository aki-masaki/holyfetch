#include "data/fetch.h"

#include "collectors/general.h"

void perform_fetch(fetch_data *out) {
  get_os_info(&out->os);
  sysinfo(&out->sys);

  get_username(out->username);
  get_hostname(out->hostname);

  out->pkgcnt = get_packagescount();
}
