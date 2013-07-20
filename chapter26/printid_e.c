#include <stdio.h>
#include <pthread.h>
#include <error.h>
#include <string.h>

pthread_t ntid;//new thread ID
void printfids(const char *s)
{
	pid_t pid;
	pthread_t tid;

	pid = getpid();
	tid = pthread_self();
	printf("%s pid %u tid %u (0x%x)\n",s,(unsigned)pid,(unsigned)tid,(unsigned)tid);
	//return;
}

void* thread_fn(void *arg)
{
	printfids("new thread:");
	return ((void *)NULL);
}
int main(int argc,char **argv)
{
	int err;
	err = pthread_create(&ntid,NULL,thread_fn,NULL);
	if(err!=0)
	{
		printf("can't create thread: %s\n",strerror(err));
		return -1;
	}
	printfids("main thread:");
//	sleep(1);
	
	
	return 0;
}
