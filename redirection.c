#include "redirection.h"

void check_for_pipe (int ncmds) {
	if (outfile || infile || appfile) {
		redirect (ncmds - 1, ncmds);
	}
}

void redirect (int i, int ncmds) {
	if (outfile && i == ncmds - 1) {
		duplicate (outfile, STDOUT, WRITE);
	}
	else if (appfile && i == ncmds - 1) {
		duplicate (appfile, STDOUT, APPEND);
	}

	if (infile && i == 0) {
		duplicate (infile, STDIN, READ);
	}
}

void duplicate (char * file, int newfd, Mode mode) {
	int oldfd = 0;
	switch (mode) {
		case (WRITE):
			oldfd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			break;
		case (APPEND):
			oldfd = open(file, O_WRONLY | O_APPEND | O_CREAT, 0600);
			break;
		case (READ):
			oldfd = open(file, O_RDONLY);
			break;
	}
	if (oldfd == -1) {
		fprintf(stderr, "shell : ");
		perror(file);
		exit (EXIT_CODE);
	}
	dup2(oldfd, newfd);
	close(oldfd);
}

