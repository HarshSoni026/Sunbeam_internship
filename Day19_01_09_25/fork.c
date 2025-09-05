#include <unistd.h>
#include <stdio.h>

int main() {
	int ret;
	printf("program started..\n");
	ret = fork();
	if(ret == 0) {
		printf("child: fork() returned: %d\n", ret);
		printf("child: pid=%d\n", getpid());
		printf("child: parent pid=%d\n", getppid());
	}
	else {
		printf("parent: fork() returned: %d\n", ret);
		printf("parent: pid=%d\n", getpid());
		printf("parent: parent pid=%d\n", getppid());
	}
	printf("program completed..\n");
	getchar();
	return 0;
}