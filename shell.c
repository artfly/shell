#include "shell.h"
 
char *infile, *outfile, *appfile;
struct command cmds[MAXCMDS];
char bkgrnd;


void handler (int sig);


int main(int argc, char *argv[]) {
	queue_t q = malloc (sizeof(queue_t));
	init(q);
	register int i;
	int oldfd; 
	char line[1024];      /*  allow large command lines  */
	int ncmds;
	char prompt[50];      /* shell prompt */
	//void (*istat)(int), (*qstat)(int);
 
      /* PLACE SIGNAL CODE HERE */
	static siginfo_t infop;

 
    sprintf(prompt,"[%s] ", argv[0]);


    struct sigaction act;
    act.sa_handler = SIG_IGN;
    struct sigaction dfl_act;
	dfl_act.sa_handler = SIG_DFL;
	sigaction (SIGINT, &act, NULL);
	sigaction (SIGQUIT, &act, NULL);
  
    while (promptline(prompt, line, sizeof(line)) > 0) {    /*until eof  */
		if ((ncmds = parseline(line)) <= 0)
    	    continue;   /* read next line */
	
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
	 	
	 //	pid_t cpid;

	 for (i = 0; i < ncmds; i++) {
		pid_t pid;
		while (waitpid(-1, (int *)0, WNOHANG) > 0);
		if (check_if_fg_or_bg(cmds[i], q)) {
			continue;
		}

		if (((pid = fork()) == 0)) {
			if (bkgrnd) {
				close(1);
				setpgid(0, 0);
			}
			else {
				sigaction (SIGTTOU, &act, NULL);
				setpgid(0,0);
				tcsetpgrp(0, getpid());
				sigaction (SIGTTOU, &dfl_act, NULL);
			}
			if (outfile && i == ncmds - 1) {
				if ((oldfd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1) {
					perror(outfile);
					exit(EXIT_CODE);
				}
				//close(1);
				dup2(oldfd, 1);
				close(oldfd);
			}
			else if (appfile && i == ncmds - 1) {
				if ((oldfd = open(appfile, O_WRONLY | O_APPEND | O_CREAT, 0600)) == -1) {
					perror(appfile);
					exit(EXIT_CODE);
				}
				dup2(oldfd, 1);
				close(oldfd);
			}

			if (infile && i == 0) {
				if ((oldfd = open(infile, O_RDONLY)) == -1) {
					perror(infile);
					exit(EXIT_CODE);
				}
				dup2(oldfd, 0);
				close(oldfd);
			}

			execvp(cmds[i].cmdargs[0], cmds[i].cmdargs);
			fprintf(stderr, "shell: %s : command not found\n", cmds[i].cmdargs[0]);
			exit (EXEC_FAILURE);
		}
		else {
			if (bkgrnd) {
				push (pid, q);
			}
			else {
				setpgid(pid, pid);
				waitid (P_PID, pid, &infop, WEXITED | WSTOPPED);
				sigaction (SIGTTOU, &act, NULL);
				if (infop.si_code == CLD_STOPPED) {
					tcsetpgrp(0, getpid());
					push (pid, q);
				}
				tcsetpgrp(0, getpid());
			}

		}


	 	/*if (bkgrnd) {
	 		//close(0);
	 		sigaction (SIGINT, &act, NULL);
			sigaction (SIGQUIT, &act, NULL);
			tcsetpgrp(0, getpid());
	 		if (((pid = fork()) == 0)) {
				setpgid(0, 0);
				execvp(cmds[i].cmdargs[0], cmds[i].cmdargs);
				fprintf(stderr, "shell: %s : command not found\n", cmds[i].cmdargs[0]);
				exit (EXEC_FAILURE);
			}
			else {
				push (pid, q);
				//setpgid(pid, pid);
			}	
	 	}
	 	else {
	 		sigaction (SIGTTOU, &act, NULL);
			if ((pid = fork()) == 0) {
				setpgid(0,0);
				tcsetpgrp(0, getpid());
				sigaction (SIGTTOU, &dfl_act, NULL);
				execvp (cmds[i].cmdargs[0], cmds[i].cmdargs);
				fprintf(stderr, "shell: %s : command not found\n", cmds[i].cmdargs[0]);
				exit (EXEC_FAILURE);
			}
			else {
				setpgid(pid, pid);
				waitid (P_PID, pid, &infop, WEXITED | WSTOPPED);
				if (infop.si_code == CLD_STOPPED) {
					tcsetpgrp(0, getpid());
					push (pid, q);
				}
				tcsetpgrp(0, getpid());
			}
	 	}*/
	 }

	}  /* close while */
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