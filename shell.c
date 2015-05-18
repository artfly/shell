#include "shell.h"
 
char *infile, *outfile, *appfile;
struct command cmds[MAXCMDS];
char bkgrnd;


void handler (int sig);


int main(int argc, char *argv[]) {
	int pipes[MAXCMDS - 1][2];
	pid_t pipe_pid;
	queue_t q = malloc (sizeof(queue_t));
	init(q);
	int i;
	int oldfd; 
	char line[1024];      
	int ncmds;
	char prompt[50];     
	siginfo_t infop;
	act.sa_handler = SIG_IGN;
	dfl_act.sa_handler = SIG_DFL;
	sigaction (SIGTTOU, &act, NULL);
	sigaction (SIGINT, &act, NULL);
	tcsetpgrp(0, getpid());

    sprintf(prompt,"[%s] ", argv[0]); 
    while (promptline(prompt, line, sizeof(line)) > 0) {    
		if ((ncmds = parseline(line)) <= 0)
			continue;   
	    #ifdef DEBUG 
	    {
	    	int i, j;
	    	for (i = 0; i < ncmds; i++) {
	        	for (j = 0; cmds[i].cmdargs[j] != (char *) NULL; j++)
	        		fprintf(stderr, "cmd[%d].cmdargs[%d] = %s\n",
	               	i, j, cmds[i].cmdargs[j]);
	          		fprintf(stderr, "cmds[%d].cmdflag = %o\n", i, cmds[i].cmdflag);
	      	}
	  	}
	    #endif
	 	

		for (i = 0; i < ncmds; i++) {
		 	sigaction (SIGTTOU, &act, NULL);
		 	if (check_if_fg_or_bg(cmds[i], q)) {
				break;
			}
			if (!create_pipe(i, pipes)) {
				break;
			}
			pid_t pid;
			while (waitpid(-1, (int *)0, WNOHANG) > 0);
			/*CHILD*/
			if (((pid = fork()) == 0)) {
				set_group(i, pid, pipe_pid);				
				if (bkgrnd) {
					set_bg_actions ();
				}
				redirect (i, ncmds);
				set_pipe(i, pipes);

				sigaction (SIGTTOU, &dfl_act, NULL);
				sigaction (SIGINT, &dfl_act, NULL);	
				execvp(cmds[i].cmdargs[0], cmds[i].cmdargs);
				fprintf(stderr, "shell: %s : command not found\n", cmds[i].cmdargs[0]);
				exit (EXEC_FAILURE);
			}
			/*PARENT*/
			else {
				setpgid (pid, pid);
				close_pipe(i, pipes);
				if (i == 0 && (cmds[i].cmdflag & OUTPIP)) {
					pipe_pid = pid;
				}
				if (bkgrnd) {
					if(!(cmds[i].cmdflag & INPIP)) {
						push (pid, q);
					}
				}
				else {
					wait_proc(i, q, pid, pipe_pid);
				}
			}
			tcsetpgrp(0, getpid());
		}
	} 
 	exit (0);
}


void handler (int sig) {
	if (sig == SIGTTOU) {
		struct sigaction act;
   		act.sa_handler = SIG_IGN;
		sigaction (SIGTSTP, &act, NULL);
		tcsetpgrp(0, getpid());
		fprintf(stderr, "%d\n", getpid());
	}
}