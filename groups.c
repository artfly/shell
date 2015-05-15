#include "groups.h"


struct sigaction act = {.sa_handler = SIG_IGN};
//act.sa_handler = SIG_IGN;
struct sigaction dfl_act = {.sa_handler = SIG_DFL};
//dfl_act.sa_handler = SIG_DFL;

void set_group (int i, int pid, int pipe_pid) {
	if ((cmds[i].cmdflag & INPIP)) {
		setpgid(0, pipe_pid);
	}
	else {
		setpgid(0, 0);
	}

	if (!bkgrnd) {
		if (!(cmds[i].cmdflag & INPIP)) {
			sigaction (SIGTTOU, &act, NULL);
			tcsetpgrp(0, getpid());
			printf("Here?\n");
			sigaction (SIGTTOU, &dfl_act, NULL);
		}
	}
}

void set_bg_actions () {
		//struct sigaction act;
//act.sa_handler = SIG_IGN;
	sigaction (SIGINT, &act, NULL);
	sigaction (SIGQUIT, &act, NULL);
}
