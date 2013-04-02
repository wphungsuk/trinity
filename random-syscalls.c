/*
 * Call random syscalls with random args.
 */

#include <stdlib.h>
#include <unistd.h>

#include "trinity.h"	// biarch
#include "child.h"
#include "syscall.h"
#include "log.h"
#include "random.h"
#include "shm.h"
#include "signals.h"
#include "pids.h"

int do_random_syscalls(int childno)
{
	pid_t pid = getpid();
	int ret;
	unsigned int syscallnr;

	sigsetjmp(ret_jump, 1);

	ret = 0;

	while (shm->exit_reason == STILL_RUNNING) {

		check_parent_pid();

		while (shm->regenerating == TRUE)
			sleep(1);

		/* If the parent reseeded, we should reflect the latest seed too. */
		if (shm->seed != shm->seeds[childno])
			set_seed(childno);

		if (biarch == TRUE) {

			/* First, check that we have syscalls enabled in either table. */
			if (validate_syscall_table_64() == FALSE) {
				use_64bit = FALSE;
				/* If no 64bit syscalls enabled, force 32bit. */
				shm->do32bit[childno] = TRUE;
			}

			if (validate_syscall_table_32() == FALSE)
				use_32bit = FALSE;

			/* If both tables enabled, pick randomly. */
			if ((use_64bit == TRUE) && (use_32bit == TRUE)) {
				/*
				 * 10% possibility of a 32bit syscall
				 */
				shm->do32bit[childno] = FALSE;

// FIXME: I forgot why this got disabled. Revisit.

//				if (rand() % 100 < 10)
//					shm->do32bit[childno] = TRUE;
			}


			if (shm->do32bit[childno] == FALSE) {
				syscalls = syscalls_64bit;
				max_nr_syscalls = max_nr_64bit_syscalls;
			} else {
				syscalls = syscalls_32bit;
				max_nr_syscalls = max_nr_32bit_syscalls;
			}
		}

		if (no_syscalls_enabled() == TRUE) {
			output(0, "[%d] No more syscalls enabled. Exiting\n", pid);
			shm->exit_reason = EXIT_NO_SYSCALLS_ENABLED;
		}

retry:
		if (shm->exit_reason != STILL_RUNNING)
			goto out;

		syscallnr = rand() % max_nr_syscalls;

		if (!(syscalls[syscallnr].entry->flags & ACTIVE))
			goto retry;

		if (syscalls[syscallnr].entry->flags & AVOID_SYSCALL)
			goto retry;

		if (syscalls[syscallnr].entry->num_args == 0)
			goto retry;

		if (syscalls[syscallnr].entry->flags & NI_SYSCALL)
			goto retry;

		/* if we get here, syscallnr is finally valid */

		shm->syscallno[childno] = syscallnr;

		if (syscalls_todo) {
			if (shm->total_syscalls_done >= syscalls_todo) {
				output(0, "[%d] shm->total_syscalls_done (%d) >= syscalls_todo (%d)\n",
					pid, shm->total_syscalls_done,syscalls_todo);
				shm->exit_reason = EXIT_REACHED_COUNT;
			}

			if (shm->total_syscalls_done == syscalls_todo)
				printf("[%d] Reached maximum syscall count %ld\n",
					pid, shm->total_syscalls_done);
		}

		ret = mkcall(childno);
	}
out:
	return ret;
}