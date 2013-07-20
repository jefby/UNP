/*
	get the exit code from thread
*/
#include <stdio.h>
#include <error.h>
#include <string.h>
#include <pthread.h>
//just return from start routine 
void *thread_fn1(void *arg)
{
	printf("thread 1 returning\n");
	return ((void *)1);
}
//pthread_exit exit 
void *thread_fn2(void *arg)
{
	printf("thread 2 exiting\n");
	pthread_exit((void*)2);
}
int main(int argc,char **argv)
{
	int err;
	pthread_t tid1,tid2;
	void *trd;

	err = pthread_create(&tid1,NULL,thread_fn1,NULL);
	if(err!=0)
	{
		printf("can't create thread 1:%s\n",strerror(err));
		return -1;
	}
	err = pthread_create(&tid2,NULL,thread_fn2,NULL);
	if(err != 0)
	{
		printf("can't create thread 2:%s\n",strerror(err));
		return -1;
	}
	err = pthread_join(tid1,&trd);
	if(err != 0)
	{
		printf("can't join tid1 to main thread%s\n",strerror(err));
		return -1;
	}
	printf("thread 1 exit code is %d\n",(int)trd);
	err = pthread_join(tid2,&trd);
	if(err != 0)
	{
		printf("can't join tid 1 to main thread%s\n",strerror(err));
		return -1;
	}
	printf("thread 2 exit code is %d\n",(int)trd);
	return (0); 
}
