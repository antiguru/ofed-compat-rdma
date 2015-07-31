#ifndef _COMPAT_LINUX_PERCPU_H
#define _COMPAT_LINUX_PERCPU_H 1

#include <linux/version.h>

#include_next <linux/percpu.h>

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,33))

#ifndef this_cpu_inc
#define this_cpu_inc(pcp)                       \
do {                                            \
        unsigned long flags;                    \
        raw_local_irq_save(flags);              \
        *this_cpu_ptr(&(pcp)) += 1;         \
        raw_local_irq_restore(flags);           \
} while (0)
#endif /* this_cpu_inc */
#endif /* (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,33)) */

#endif	/* _COMPAT_LINUX_PERCPU_H */
