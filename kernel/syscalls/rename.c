#include <hermit/syscall.h>
#include <hermit/logging.h>
#include <asm/page.h>
#include <asm/uhyve.h>
#include <hermit/minifs.h>

typedef struct {
	char *old;
	char *new;
	int ret;
} __attribute__((packed)) uhyve_rename_t;

int sys_rename(const char *old,const char *new) {
	uhyve_rename_t arg;

	if(minifs_enabled)
		return minifs_creat(path, mode);

	arg.old = (char *)virt_to_phys((size_t)old);
	arg.new (char *)virt_to_phys((size_t)new);
	arg.ret = -1;

	uhyve_send(UHYVE_PORT_RENAME, (unsigned)virt_to_phys((size_t)&arg));

	return arg.ret;
}
