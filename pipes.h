#ifndef PIPES_H
#define PIPES_H

#include "shell.h"
#include "redirection.h"

int create_pipe(int i, int pipes[MAXCMDS - 1][2]);
void set_pipe (int i, int pipes[MAXCMDS - 1][2]);
void close_pipe (int i, int pipes[MAXCMDS - 1][2]);
void dup_pipe (int pipefd, int newfd);

#endif