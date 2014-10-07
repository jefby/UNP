/*
	UNP chapter 1 example : request to the server for current time
	Author:jefby
	Email:jef199006@gmail.com
*/
#include <netinet/in.h>
#include <stdio.h>
#define MAXLINE 4096
#define SA struct socketaddr_in
#include <sys/socket.h>

int main(int argc,char **argv)
{
	//sockfd:套接字描述符
	int sockfd,n;
	//接收缓冲
	char recvline[MAXLINE+1];
	//套接字IPV4
	struct sockaddr_in	servaddr,cliaddr;
	socklen_t len;

	if(argc != 2)
	{
		printf("usage: a.out <IP address>");
		exit(-1);
	}
	//创建一个网际字节流
	if((sockfd=socket(AF_INET,SOCK_STREAM,0)) < 0)
		printf("socket error");
	//初始化为0
	bzero(&servaddr,sizeof(servaddr));
	
	servaddr.sin_family = AF_INET;
	//设置服务器端口；获取时间为熟知端口13
	servaddr.sin_port = htons(13);
	//将命令行中的IP地址转换为数值
	if(inet_pton(AF_INET,argv[1],&servaddr.sin_addr) <= 0)
	{
		printf("inet_pton error for %s",argv[1]);
		return -1;
	}
	//客户端向服务器发出请求，建立连接	
	if(connect(sockfd,(SA*)&servaddr,sizeof(servaddr)) <0)
	{
		printf("connect error");
		return -1;
	}
	//len = sizeof(cliaddr) is very very import,can't be removed
	len = sizeof(cliaddr);
	//获取本地协议地址,将本地套接字保存在cliaddr上
	if(0!=getsockname(sockfd,(SA*)&cliaddr,&len))
		printf("getsockname error!\n");
	//输出本地协议地址
	printf("local socket is : %s  port %d\n",inet_ntop(AF_INET,&cliaddr.sin_addr,recvline,sizeof(recvline)),ntohs(cliaddr.sin_port));
	while((n=read(sockfd,recvline,MAXLINE))>0)
	{
		recvline[n]=0;	//null terminate
		if(fputs(recvline,stdout)==EOF)//输出接收到的数据
		{
			printf("fputs error");
			return -1;
		}
	}
	//if n==0,closed the connect normally;else error
	if(n < 0)
		printf("read error");
	exit(0);
}
