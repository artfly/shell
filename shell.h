#ifndef SHELL_H
#define SHELL_H

#define MAXARGS 256
#define MAXCMDS 50

struct command {
    char *cmdargs[MAXARGS];
    char cmdflag;
};

#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <unistd.h>
#include <wait.h>
#include <termios.h>
#include "queue.h"
#include "groups.h"
#include "job_control.h"
#include "redirection.h"
#include "pipes.h"


#define EXIT_CODE 1
#define EXEC_FAILURE 2
#define FORK_FAILURE 3
#define OUTPIP  01
#define INPIP   02
#define STDIN 0
#define STDOUT 1


extern struct command cmds[];
extern struct queue q;
extern char *infile, *outfile, *appfile;
extern char bkgrnd;

int parseline(char *);
int promptline(char *, char *, int);

#endif
