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
	killpg (pid, SIGCONT);
}

void fg (int job, queue_t q) {
	siginfo_t infop;
	pid_t pid = get(job, q);
	tcsetpgrp(0, pid);
	sigaction (SIGINT, &dfl_act, 0);
	killpg (pid, SIGCONT);
	while (waitid (P_PGID, pid, &infop, WEXITED | WSTOPPED) != 0);
	if (infop.si_code == CLD_STOPPED) {
		push (pid, q);
	}
	tcsetpgrp(0, getpid());
	sigaction (SIGINT, &act, 0);
}

void wait_proc(int i, queue_t q, int pid, int pipe_pid) {
	siginfo_t infop;
	if (!(cmds[i].cmdflag & OUTPIP)) {
		waitid (P_PID, pid, &infop, WEXITED | WSTOPPED);
		if (infop.si_code == CLD_STOPPED) {
			push (getpgid(pid), q);
		}
	}
}