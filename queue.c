#include "queue.h"

	void init (queue_t q) {
		q->jobs = (pid_t *) malloc (sizeof(pid_t) * MAX_SIZE);
		q->last = 1;
	}

	int push(pid_t job, queue_t q) {
 		if (q->last == MAX_SIZE) {
 			fprintf(stderr, "shell : cannot add job to bg\n");
 			return -1;
 		}
 		q->jobs[q->last] = job;
 		fprintf(stderr, "\n[%d] %d\n", q->last, job);
 		q->last++;
 		return 0;
	}

	pid_t pop(queue_t q) {
		if (q->last == 1) {
			fprintf (stderr, "shell : no bg jobs\n");
			return -1;
		}
		q->last--;
		return q->jobs[q->last];
	}

	void shift_queue (int from, queue_t q) {
		int i = 0;
		for (i = from + 1; i < q->last; i++) {
			q->jobs[i - 1] = q->jobs[i]; 
		}
		q->last--;
	} 

	pid_t get (int index, queue_t q) {
		if (index < 1 || q->last <= index) {
			printf("shell : no such job \n");
			return -1;
		}
		pid_t pid = q->jobs[index];
		shift_queue (index, q);
		return pid;
	}

	void delete(queue_t q) {
		free (q->jobs);
	}