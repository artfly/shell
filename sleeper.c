#include <signal.h>
#include <unistd.h>
#include <stdio.h>

int main (int argc, char * argv[]) {
	sleep (5);
	printf ("We waited enough\n");
	printf ("Other stuff\n");
	printf("SLEEP ID : %d\n", getpid());
	return 0;
}
