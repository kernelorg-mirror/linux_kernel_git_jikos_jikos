#ifndef ASM_KGR_H
#define ASM_KGR_H

#include <linux/linkage.h>

/*
 * The stub needs to modify the RIP value stored in struct pt_regs
 * so that ftrace redirects the execution properly.
 */
#define KGR_STUB_ARCH_SLOW(_name, _new_function)			\
static void _new_function ##_stub_slow (unsigned long ip, unsigned long parent_ip,	\
		struct ftrace_ops *ops, struct pt_regs *regs)		\
{									\
	struct kgr_loc_caches *c = ops->private;			\
									\
	if (task_thread_info(current)->kgr_in_progress && current->mm) {\
		pr_info("kgr: slow stub: calling old code at %lx\n",	\
				c->old);				\
		regs->ip = c->old + MCOUNT_INSN_SIZE;			\
	} else {							\
		pr_info("kgr: slow stub: calling new code at %lx\n",	\
				c->new);				\
		regs->ip = c->new;					\
	}								\
}

#define KGR_STUB_ARCH_FAST(_name, _new_function)			\
static void _new_function ##_stub_fast (unsigned long ip,		\
		unsigned long parent_ip, struct ftrace_ops *ops,	\
		struct pt_regs *regs)					\
{									\
	struct kgr_loc_caches *c = ops->private;			\
									\
	BUG_ON(!c->new);				\
	pr_info("kgr: fast stub: calling new code at %lx\n", c->new); \
	regs->ip = c->new;				\
}

#endif
