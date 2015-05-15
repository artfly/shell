#ifndef REDIRECT_H
#define REDIRECT_H

#include "shell.h"

enum Mode {
	READ, WRITE, APPEND
};
typedef enum Mode Mode;

void redirect (int i, int ncmds);
void duplicate (char * file, int newfd, Mode mode);


#endif