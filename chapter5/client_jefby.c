/*
	Author:jefby
	Email:jef199006@gmail.com
*/
#include <stdio.h>
#include <netinet/in.h>

#define SA struct sockaddr_in
#define SERV_PORT 9887
#define MAXLINE 4096

ssize_t readline(int fd,void *vptr,size_t maxlen);

void str_cli(FILE *fp,int sockfd)
{
	char sendline[MAXLINE],recvline[MAXLINE];
	//from file pointer get a line 
	while(fgets(sendline,MAXLINE,fp) != NULL)
	{
		//write the sendfile's context to the sockfd
		write(sockfd,sendline,strlen(sendline));
		//read from server 
		if(readline(sockfd,recvline,MAXLINE) == 0)
		{
			printf("str_cli: server terminated prematurely!\n");
			exit(1);
		}
		//print the recvline to the stdout
		fputs(recvline,stdout);
	}
	
}
int main(int argc,char **argv)
{
	int sockfd;
	struct sockaddr_in servaddr;
	
	if(argc != 2)
	{
		printf("usage : a.out <IP address>\n");
		exit(1);
	}
	//create a socket for ipv4 
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	//clear the servaddr to 0
	bzero(&servaddr,sizeof(servaddr));
	//set the AF_INET
	servaddr.sin_family = AF_INET;
	//set the server port
	servaddr.sin_port = htons(SERV_PORT);
	//convert the ip to number
	inet_pton(AF_INET,argv[1],&servaddr.sin_addr);
	//construct the connection
	connect(sockfd,(SA*)&servaddr,sizeof(servaddr));
	str_cli(stdin,sockfd);
	

	return 0;
}
