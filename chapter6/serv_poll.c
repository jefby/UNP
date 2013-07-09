/*
	server example 
	Author:jefby
	Email:jef199006@gmail.com
*/
#include <stdio.h>
#include <netinet/in.h>
//#include <stdlib.h>
#include <errno.h>
#include <signal.h>
//#include <sys/types.h>
#include <sys/wait.h>
#include <poll.h>
//#include <limits.h>
#define INFTIM -1
#define OPEN_MAX 1024
#define LISTENQ 1024
#define SERV_PORT 9887
#define SA struct sockaddr_in
#define MAXLINE 4096

typedef void Sigfunc(int);

Sigfunc * signal(int signo,Sigfunc *func)
{
	struct sigaction act,oact;
	act.sa_handler = func;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	if(signo == SIGALRM)
	{
	#ifdef SA_INTERRUPT
	act.sa_flags |= SA_INTERRUPT;//SunOS 4.X
	#endif
	}
	else
	{
	#ifdef SA_RESTART
	act.sa_flags |= SA_RESTART;//SVR4,4.4BSD
	#endif	
	}
	if(sigaction(signo,&act,&oact) < 0)
		return (SIG_ERR);
	return (oact.sa_handler);
}

void sig_chld(int signo)
{
	pid_t pid;
	int stat;
	
	while(	(pid = waitpid(-1,&stat,WNOHANG)) > 0)
		printf("child %d  terminated\n",pid);
	return;
}
void sig_pipe(int signo)
{
	printf("broken pipe\n");
	return;
}
void str_echo(int sockfd)
{
	ssize_t n;
	char buf[MAXLINE];

	//read from client and echo it to client	
	again:
	while((n=read(sockfd,buf,MAXLINE))>0)
	{
		write(sockfd,buf,n);
	}
	if(n<0&&errno==EINTR)
		goto again;
	else if(n < 0)
	{
		printf("str_echo:read error\n");
		exit(1);
	}
	
}
int main(int argc,char **argv)
{
	int i,maxi,maxfd,sockfd;
	ssize_t n;
	char buf[MAXLINE];
	int listenfd,connfd;
	pid_t childpid;
	socklen_t clilen;
	struct sockaddr_in cliaddr,servaddr;
	int nready;
	struct pollfd client[OPEN_MAX];

	//create a socket
	listenfd = socket(AF_INET,SOCK_STREAM,0);
	//clear to this socket to 0
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	//listen to any ip address
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servaddr.sin_port=htons(SERV_PORT);//specified in .c
	bind(listenfd,(SA*)&servaddr,sizeof(servaddr));//bind to listenfd 
	listen(listenfd,LISTENQ);//listen 
	
	client[0].fd = listenfd;
	client[0].events = POLLRDNORM;
	for(i=1;i<OPEN_MAX;++i)
		client[i].fd = -1;//-1 indicates available entry
	maxi = 0;	//max index into client[] array

	while(1)
	{
		nready = poll(client,maxi+1,INFTIM);
		
		if(client[0].revents & POLLRDNORM)//new connection
		{
			clilen = sizeof(cliaddr);
			connfd = accept(listenfd,(SA*)&cliaddr,&clilen);
			for(i=1;i<OPEN_MAX;++i)
			{
				if(client[i].fd < 0)
				{
					client[i].fd = connfd;//save descriptor
					break;
				}
			}
			if(i == OPEN_MAX)
			{
				printf("too many clients\n");
				exit(1);
			}
			client[i].events = POLLRDNORM;
			if(i > maxi)
				maxi = i;
			if(--nready <= 0)
				continue;//no more readale descriptors
		}
		for(i=1;i<=maxi;++i)
		{
			if((sockfd=client[i].fd)<0)
				continue;
			if(client[i].revents &(POLLRDNORM | POLLERR))
			{
				if((n=read(sockfd,buf,MAXLINE))<0)
				{
					if(errno==ECONNRESET)
					{
					//connection reset by client
						close(sockfd);
						client[i].fd = -1;				
					}
					else
					{
						printf("read error!\n");
						exit(1);
					}
				}
				else if(n == 0)//connection closed by client
				{
					close(sockfd);
					client[i].fd = -1;
				}
				else
					write(sockfd,buf,n);
				if(--nready <= 0)
					break;//no more readable descriptors
			}
		}
	}	
	return 0;
}
