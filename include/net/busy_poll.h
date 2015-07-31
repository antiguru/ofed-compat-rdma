#ifndef _COMPAT_LINUX_NET_BUSY_POLL_H
#define _COMPAT_LINUX_NET_BUSY_POLL_H 1

#include <linux/version.h>

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,11,0))
#include_next <net/busy_poll.h>
#endif

#endif	/* _COMPAT_LINUX_NET_BUSY_POLL_H */
