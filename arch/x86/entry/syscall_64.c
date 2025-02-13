// SPDX-License-Identifier: GPL-2.0
/* System call table for x86-64. */

#include <linux/linkage.h>
#include <linux/sys.h>
#include <linux/cache.h>
#include <asm/asm-offsets.h>
#include <asm/syscall.h>

/* this is a lie, but it does not hurt as sys_ni_syscall just returns -EINVAL */
extern asmlinkage long sys_ni_syscall(const struct pt_regs *);
#define __SYSCALL_64(nr, sym, qual) extern asmlinkage long sym(const struct pt_regs *);
#define __SYSCALL_X32(nr, sym, qual) __SYSCALL_64(nr, sym, qual)
#include <asm/syscalls_64.h>
#undef __SYSCALL_64
#undef __SYSCALL_X32

#define __SYSCALL_64(nr, sym, qual) [nr] = sym,
#define __SYSCALL_X32(nr, sym, qual)

asmlinkage const sys_call_ptr_t sys_call_table[__NR_syscall_max+1] = {
	/*
	 * Smells like a compiler bug -- it doesn't work
	 * when the & below is removed.
	 */
	[0 ... __NR_syscall_max] = &sys_ni_syscall,
#include <asm/syscalls_64.h>
};

EXPORT_SYMBOL(sys_call_table);

#undef __SYSCALL_64
#undef __SYSCALL_X32

#ifdef CONFIG_X86_X32_ABI

#define __SYSCALL_64(nr, sym, qual)
#define __SYSCALL_X32(nr, sym, qual) [nr] = sym,

asmlinkage const sys_call_ptr_t x32_sys_call_table[__NR_syscall_x32_max+1] = {
	/*
	 * Smells like a compiler bug -- it doesn't work
	 * when the & below is removed.
	 */
	[0 ... __NR_syscall_x32_max] = &sys_ni_syscall,
#include <asm/syscalls_64.h>
};

#undef __SYSCALL_64
#undef __SYSCALL_X32

#endif
