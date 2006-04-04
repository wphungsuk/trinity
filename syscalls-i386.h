/* Syscalls from arch/i386/kernel/entry.S as of 2.6.17rc1 */

#include "scrashme.h"
#include "sanitise.h"

#define NR_SYSCALLS 314

struct syscalltable syscalls_i386[NR_SYSCALLS+1] = {
	{ .name = "restart_syscall", },	/* 0 - old "setup()" system call, used for restarting */
	{ .name = "exit", },
	{ .name = "fork", },
	{ .name = "read", .sanitise = sanitise_read },
	{ .name = "write", .sanitise = sanitise_write },
	{ .name = "open", },		/* 5 */
	{ .name = "close", },
	{ .name = "waitpid", },
	{ .name = "creat", },
	{ .name = "link", },
	{ .name = "unlink", },	/* 10 */
	{ .name = "execve", },
	{ .name = "chdir", },
	{ .name = "time", },
	{ .name = "mknod", },
	{ .name = "chmod", },		/* 15 */
	{ .name = "lchown16", },
	{ .name = "ni_syscall (old break syscall holder)", },
	{ .name = "stat", },
	{ .name = "lseek", },
	{ .name = "getpid", },	/* 20 */
	{ .name = "mount", },
	{ .name = "oldumount", },
	{ .name = "setuid16", },
	{ .name = "getuid16", },
	{ .name = "stime", },		/* 25 */
	{ .name = "ptrace", },
	{ .name = "alarm", },
	{ .name = "fstat", },
	{ .name = "pause", },
	{ .name = "utime", },		/* 30 */
	{ .name = "ni_syscall (old stty syscall holder)", },
	{ .name = "ni_syscall (old gtty syscall holder", },
	{ .name = "access", },
	{ .name = "nice", },
	{ .name = "ni_syscall (old ftime syscall holder)", },	/* 35 */
	{ .name = "sync", },
	{ .name = "kill", },
	{ .name = "rename", },
	{ .name = "mkdir", },
	{ .name = "rmdir", },		/* 40 */
	{ .name = "dup", },
	{ .name = "pipe", },
	{ .name = "times", },
	{ .name = "ni_syscall (old prof syscall holder)", },
	{ .name = "brk", },		/* 45 */
	{ .name = "setgid16", },
	{ .name = "getgid16", },
	{ .name = "signal", },
	{ .name = "geteuid16", },
	{ .name = "getegid16", },	/* 50 */
	{ .name = "acct", },
	{ .name = "umount (recycled never used phys())", },
	{ .name = "ni_syscall (old lock syscall holder", },
	{ .name = "ioctl", },
	{ .name = "fcntl", },		/* 55 */
	{ .name = "ni_syscall (old mpx syscall holder)", },
	{ .name = "setpgid", },
	{ .name = "ni_syscall (old ulimit syscall holder)", },
	{ .name = "olduname", },
	{ .name = "umask", },		/* 60 */
	{ .name = "chroot", },
	{ .name = "ustat", },
	{ .name = "dup2", },
	{ .name = "getppid", },
	{ .name = "getpgrp", },	/* 65 */
	{ .name = "setsid", },
	{ .name = "sigaction", },
	{ .name = "sgetmask", },
	{ .name = "ssetmask", },
	{ .name = "setreuid16", },	/* 70 */
	{ .name = "setregid16", },
	{ .name = "sigsuspend", },
	{ .name = "sigpending", },
	{ .name = "sethostname", },
	{ .name = "setrlimit", },	/* 75 */
	{ .name = "old_getrlimit", },
	{ .name = "getrusage", },
	{ .name = "gettimeofday", },
	{ .name = "settimeofday", },
	{ .name = "getgroups16", },	/* 80 */
	{ .name = "setgroups16", },
	{ .name = "select", },
	{ .name = "symlink", },
	{ .name = "lstat", },
	{ .name = "readlink", },	/* 85 */
	{ .name = "uselib", },
	{ .name = "swapon", },
	{ .name = "reboot", },
	{ .name = "readdir", },
	{ .name = "mmap", },		/* 90 */
	{ .name = "munmap", },
	{ .name = "truncate", },
	{ .name = "ftruncate", },
	{ .name = "fchmod", },
	{ .name = "fchown16", },	/* 95 */
	{ .name = "getpriority", },
	{ .name = "setpriority", },
	{ .name = "ni_syscall (old profil syscall holder)", },
	{ .name = "statfs", },
	{ .name = "fstatfs", },	/* 100 */
	{ .name = "ioperm", },
	{ .name = "socketcall", },
	{ .name = "syslog", },
	{ .name = "setitimer", },
	{ .name = "getitimer", },	/* 105 */
	{ .name = "newstat", },
	{ .name = "newlstat", },
	{ .name = "newfstat", },
	{ .name = "uname", },
	{ .name = "iopl", },		/* 110 */
	{ .name = "vhangup", },
	{ .name = "ni_syscall (old 'idle' system call)", },
	{ .name = "vm86old", },
	{ .name = "wait4", },
	{ .name = "swapoff", },	/* 115 */
	{ .name = "sysinfo", },
	{ .name = "ipc", },
	{ .name = "fsync", },
	{ .name = "sigreturn", },
	{ .name = "clone", },		/* 120 */
	{ .name = "setdomainname", },
	{ .name = "newuname", },
	{ .name = "modify_ldt", },
	{ .name = "adjtimex", },
	{ .name = "mprotect", },	/* 125 */
	{ .name = "sigprocmask", },
	{ .name = "ni_syscall (old create_module)", },
	{ .name = "init_module", },
	{ .name = "delete_module", },
	{ .name = "ni_syscall	(old get_kernel_syms)", },	/* 130 */
	{ .name = "quotactl", },
	{ .name = "getpgid", },
	{ .name = "fchdir", },
	{ .name = "bdflush", },
	{ .name = "sysfs", },		/* 135 */
	{ .name = "personality", },
	{ .name = "ni_syscall	(reserved for afs_syscall)", },
	{ .name = "setfsuid16", },
	{ .name = "setfsgid16", },
	{ .name = "llseek", },	/* 140 */
	{ .name = "getdents", },
	{ .name = "select", },
	{ .name = "flock", },
	{ .name = "msync", },
	{ .name = "readv", },	/* 145 */
	{ .name = "writev", },
	{ .name = "getsid", },
	{ .name = "fdatasync", },
	{ .name = "sysctl", },
	{ .name = "mlock", },		/* 150 */
	{ .name = "munlock", },
	{ .name = "mlockall", },
	{ .name = "munlockall", },
	{ .name = "sched_setparam", },
	{ .name = "sched_getparam", },   /* 155 */
	{ .name = "sched_setscheduler", },
	{ .name = "sched_getscheduler", },
	{ .name = "sched_yield", },
	{ .name = "sched_get_priority_max", },
	{ .name = "sched_get_priority_min", },  /* 160 */
	{ .name = "sched_rr_get_interval", },
	{ .name = "nanosleep", },
	{ .name = "mremap", },
	{ .name = "setresuid16", },
	{ .name = "getresuid16", },	/* 165 */
	{ .name = "vm86", },
	{ .name = "ni_syscall (Old sys_query_module)", },
	{ .name = "poll", },
	{ .name = "nfsservctl", },
	{ .name = "setresgid16", },	/* 170 */
	{ .name = "getresgid16", },
	{ .name = "prctl", },
	{ .name = "rt_sigreturn", },
	{ .name = "rt_sigaction", },
	{ .name = "rt_sigprocmask", },	/* 175 */
	{ .name = "rt_sigpending", },
	{ .name = "rt_sigtimedwait", },
	{ .name = "rt_sigqueueinfo", },
	{ .name = "rt_sigsuspend", },
	{ .name = "pread64", },	/* 180 */
	{ .name = "pwrite64", },
	{ .name = "chown16", },
	{ .name = "getcwd", },
	{ .name = "capget", },
	{ .name = "capset", },	/* 185 */
	{ .name = "sigaltstack", },
	{ .name = "sendfile", },
	{ .name = "ni_syscall	(reserved for streams1)", },
	{ .name = "ni_syscall	(reserved for streams2)", },
	{ .name = "vfork", },		/* 190 */
	{ .name = "getrlimit", },
	{ .name = "mmap2", },
	{ .name = "truncate64", },
	{ .name = "ftruncate64", },
	{ .name = "stat64", },	/* 195 */
	{ .name = "lstat64", },
	{ .name = "fstat64", },
	{ .name = "lchown", },
	{ .name = "getuid", },
	{ .name = "getgid", },	/* 200 */
	{ .name = "geteuid", },
	{ .name = "getegid", },
	{ .name = "setreuid", },
	{ .name = "setregid", },
	{ .name = "getgroups", },	/* 205 */
	{ .name = "setgroups", },
	{ .name = "fchown", },
	{ .name = "setresuid", },
	{ .name = "getresuid", },
	{ .name = "setresgid", },	/* 210 */
	{ .name = "getresgid", },
	{ .name = "chown", },
	{ .name = "setuid", },
	{ .name = "setgid", },
	{ .name = "setfsuid", },	/* 215 */
	{ .name = "setfsgid", },
	{ .name = "pivot_root", },
	{ .name = "mincore", },
	{ .name = "madvise", },
	{ .name = "getdents64", },	/* 220 */
	{ .name = "fcntl64", },
	{ .name = "ni_syscall	(reserved for TUX)", },
	{ .name = "ni_syscall (223)", },
	{ .name = "gettid", },
	{ .name = "readahead", },	/* 225 */
	{ .name = "setxattr", },
	{ .name = "lsetxattr", },
	{ .name = "fsetxattr", },
	{ .name = "getxattr", },
	{ .name = "lgetxattr", },	/* 230 */
	{ .name = "fgetxattr", },
	{ .name = "listxattr", },
	{ .name = "llistxattr", },
	{ .name = "flistxattr", },
	{ .name = "removexattr", },	/* 235 */
	{ .name = "lremovexattr", },
	{ .name = "fremovexattr", },
	{ .name = "tkill", },
	{ .name = "sendfile64", },
	{ .name = "futex", },		/* 240 */
	{ .name = "sched_setaffinity", },
	{ .name = "sched_getaffinity", },
	{ .name = "set_thread_area", },
	{ .name = "get_thread_area", },
	{ .name = "io_setup", },	/* 245 */
	{ .name = "io_destroy", },
	{ .name = "io_getevents", },
	{ .name = "io_submit", },
	{ .name = "io_cancel", },
	{ .name = "fadvise64", },	/* 250 */
	{ .name = "ni_syscall (251)", },
	{ .name = "exit_group", },
	{ .name = "lookup_dcookie", },
	{ .name = "epoll_create", },
	{ .name = "epoll_ctl", },	/* 255 */
	{ .name = "epoll_wait", },
	{ .name = "remap_file_pages", },
	{ .name = "set_tid_address", },
	{ .name = "timer_create", },
	{ .name = "timer_settime", },		/* 260 */
	{ .name = "timer_gettime", },
	{ .name = "timer_getoverrun", },
	{ .name = "timer_delete", },
	{ .name = "clock_settime", },
	{ .name = "clock_gettime", },		/* 265 */
	{ .name = "clock_getres", },
	{ .name = "clock_nanosleep", },
	{ .name = "statfs64", },
	{ .name = "fstatfs64", },
	{ .name = "tgkill", },	/* 270 */
	{ .name = "utimes", },
	{ .name = "fadvise64_64", },
	{ .name = "ni_syscall (reserved for vserver)", },
	{ .name = "mbind", },
	{ .name = "get_mempolicy", },
	{ .name = "set_mempolicy", },
	{ .name = "mq_open", },
	{ .name = "mq_unlink", },
	{ .name = "mq_timedsend", },
	{ .name = "mq_timedreceive", },	/* 280 */
	{ .name = "mq_notify", },
	{ .name = "mq_getsetattr", },
	{ .name = "kexec_load", },
	{ .name = "waitid", },
	{ .name = "ni_syscall", },		/* 285 */ /* available */
	{ .name = "add_key", },
	{ .name = "request_key", },
	{ .name = "keyctl", },
	{ .name = "ioprio_set", },
	{ .name = "ioprio_get", },	/* 290 */
	{ .name = "inotify_init", },
	{ .name = "inotify_add_watch", },
	{ .name = "inotify_rm_watch", },
	{ .name = "migrate_pages", },
	{ .name = "openat", },		/* 295 */
	{ .name = "mkdirat", },
	{ .name = "mknodat", },
	{ .name = "fchownat", },
	{ .name = "futimesat", },
	{ .name = "fstatat64", },		/* 300 */
	{ .name = "unlinkat", },
	{ .name = "renameat", },
	{ .name = "linkat", },
	{ .name = "symlinkat", },
	{ .name = "readlinkat", },		/* 305 */
	{ .name = "fchmodat", },
	{ .name = "faccessat", },
	{ .name = "pselect6", },
	{ .name = "ppoll", },
	{ .name = "unshare", },		/* 310 */
	{ .name = "set_robust_list", },
	{ .name = "get_robust_list", },
	{ .name = "splice", .sanitise = sanitise_splice },
	{ .name = "sync_file_range", },
};

