#include "pipes.h"

int create_pipe(int i, int pipes[MAXCMDS - 1][2]) {
	if ((cmds[i].cmdflag & OUTPIP)) {
		if (pipe(pipes[i]) == -1) {
			fprintf(stderr, "shell : error : couldn't create pipe\n");
			return 0;
		}
	}
	return 1;
}

void set_pipe (int i, int pipes[MAXCMDS - 1][2]) {
	if (cmds[i].cmdflag & OUTPIP) {
		dup_pipe (pipes[i][1], STDOUT);
	}
	if (cmds[i].cmdflag & INPIP) {
		dup_pipe (pipes[i - 1][0], STDIN);
	}
}

void close_pipe (int i, int pipes[MAXCMDS - 1][2]) {
	if (cmds[i].cmdflag & OUTPIP) {
		close (pipes[i][1]);
	}
	if (cmds[i].cmdflag & INPIP) {
		close (pipes[i - 1][0]);
	}
}

void dup_pipe (int pipefd, int newfd) {
	dup2(pipefd, newfd);
	close(pipefd);
}