all:
	gcc shell.c job_control.c queue.c parseline.c promptline.c redirection.c pipes.c groups.c -Wall -o shell -g
job_control: job_control.c job_control.h shell.h
	gcc job_control.c -Wall -o ctrl -g
queue: queue.c queue.h shell.h
	gcc queue.c queue.h -Wall -o queue -g
example: shell1.c 
	gcc shell1.c parseline.c promptline.c -Wall -o example -g


