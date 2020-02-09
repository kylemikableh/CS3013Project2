//Kyle Mikolajczyk
//kjmikolajczyk@wpi.edu

#include <sys/syscall.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

// These values MUST match the unistd_32.h modifications:
#define __NR_cs3013_syscall1 377
#define __NR_cs3013_syscall2 378
#define __NR_cs3013_syscall3 379

struct ancestry {
	pid_t ancestors[10];
	pid_t siblings[100];
	pid_t children[100];
};

long testCall1 ( void) {
	return (long) syscall(__NR_cs3013_syscall1);
}

long testCall2 (unsigned short *target_pid, struct ancestry *response) {
	return (long) syscall(__NR_cs3013_syscall2, target_pid, response);
}

long testCall3 ( void) {
	return (long) syscall(__NR_cs3013_syscall3);
}

int main (int argc, char *argv[]) {
	printf("The return values of the system calls are:\n");

	struct ancestry *myAncestry = malloc(sizeof(struct ancestry));
	if(argc < 2)
	{
		printf("Not enough arguments! Exiting...\n");
		return 0;
	}
	unsigned short pid = atoi(argv[1]);
	unsigned short* pid_ptr = &pid;
	// unsigned short *pid = malloc(sizeof(unsigned short));
	printf("\tcs3013_syscall2: %ld\n", testCall2(pid_ptr, myAncestry));
	return 0;
}
