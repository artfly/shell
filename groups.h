#ifndef GROUPS_H
#define GROUPS_H

#include "shell.h"

extern struct sigaction act;
extern struct sigaction dfl_act;


void set_group (int i, int pid, int pipe_pid);
void set_bg_actions ();

#endif