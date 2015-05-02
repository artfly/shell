#include "job_control.h"
#include <string.h>

int check_if_fg_or_bg (struct command cmd, queue_t q) {
	if (!strcmp (cmd.cmdargs[0], "fg")) {
		if (cmd.cmdargs[1]) {
			fg (atoi(cmd.cmdargs[1]), q);
		}
		else {
			fg (q->last - 1, q);
		}
		return 1;
	}
	else if (!strcmp (cmd.cmdargs[0], "bg")) {
		if (cmd.cmdargs[1]) {
			bg (atoi(cmd.cmdargs[1]), q);
		}
		else {
			bg (q->last - 1, q);
		}
		return 1;
	}
	return 0;
}


void bg (int job, queue_t q) {
	pid_t pid = get (job, q);
	push(pid,q);
	if (pid < 0) {
		return;
	}	
	kill (pid, SIGCONT);
}

void fg (int job, queue_t q) {
	static siginfo_t infop;
	pid_t pid = get(job, q);
	if (pid < 0) {
		return;
	}
	tcsetpgrp(0, pid);
	kill (pid, SIGCONT);
	waitid (P_PID, pid, &infop, WEXITED | WSTOPPED);
	if (infop.si_code == CLD_STOPPED) {
		tcsetpgrp(0, getpid());
		push (pid, q);
	}
	tcsetpgrp(0, getpid());
}