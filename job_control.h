#ifndef JOB_CTRL_H
#define JOB_CTRL_H

#include "shell.h"

int check_if_fg_or_bg (struct command cmd, queue_t q);
void bg (int job, queue_t q);
void fg (int job, queue_t q);



#endif