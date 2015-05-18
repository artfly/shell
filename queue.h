#ifndef QUEUE_H
#define QUEUE_H

#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <unistd.h>
#include <wait.h>
#define MAX_SIZE 32768

struct queue {
	pid_t * jobs;
	int last;
};
typedef struct queue * queue_t;

void init (queue_t q);
int push(pid_t job, queue_t q);
pid_t pop(queue_t q);
void shift_queue (int from, queue_t q);
pid_t get (int index, queue_t q);
void delete(queue_t q);
void pop_if_nedeed (pid_t pid, queue_t q);

#endif