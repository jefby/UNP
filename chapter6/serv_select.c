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
	int nready,client[FD_SETSIZE];
	ssize_t n;
	fd_set rset,allset;
	char buf[MAXLINE];
	int listenfd,connfd;
	pid_t childpid;
	socklen_t clilen;
	struct sockaddr_in cliaddr,servaddr;
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
	
	maxfd = listenfd;
	maxi = -1;
	for(i=0;i<FD_SETSIZE;++i)
		client[i] = -1;
	FD_ZERO(&allset);
	FD_SET(listenfd,&allset);
	while(1)
	{
		rset = allset;
		nready = select(maxfd+1,&rset,NULL,NULL,NULL);
		if(FD_ISSET(listenfd,&rset))
		{
			clilen=sizeof(cliaddr);
			connfd = accept(listenfd,(SA*)&cliaddr,&clilen);
			
			for(i=0;i<FD_SETSIZE;++i)
			{
				if(client[i] < 0)
				{
					client[i] = connfd;//save descriptor
					break;
				}
			}
			if(i == FD_SETSIZE)
			{
				printf("too many clients\n");
				exit(1);
			}
			FD_SET(connfd,&allset);//add new descriptor to set
			if(connfd > maxfd)
				maxfd = connfd;//for select
			if(i > maxi)
				maxi = i;//max index in client[] array
			if(--nready <= 0)
				continue;//no more readable descriptors
		}
		for(i=0;i<=maxi;++i)//check all clients for data
		{
			if((sockfd = client[i])<0)
				continue;
			if(FD_ISSET(sockfd,&rset))
			{
				if((n=read(sockfd,buf,MAXLINE)) == 0)
				{
				//connection closed by client
					close(sockfd);
					FD_CLR(sockfd,&allset);
					client[i]=-1;
				}
				else
					write(sockfd,buf,n);
				if(--nready <= 0)
					break;//no more readable descriptor
			}
		}
	}
	return 0;
}
