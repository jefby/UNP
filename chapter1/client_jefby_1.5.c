/*
	UNP chapter 1 example : request to the server for current time
	Author:jefby
	Email:jef199006@gmail.com
*/
#include <netinet/in.h>
#include <stdio.h>

#define MAXLINE 100
#define SA struct sockaddr_in
 
//#include "apueerror.h"
int main(int argc,char **argv)
{
	//sockfd:套接字描述符
	int sockfd,n;
	//接收缓冲
	char recvline[MAXLINE+1];
	//套接字IPV4
	SA	servaddr;
	int cnt=0;
	
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
	servaddr.sin_port = htons(9999);
	//将命令行中的ascii码点分十进制IP地址转换为数值
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
	//接收服务器传回的数据，返回0表示对端关闭连接，返回负值表示出错
	while((n=read(sockfd,recvline,MAXLINE))>0)
	{
		recvline[n]=0;	//null terminate
		++cnt;
		if(fputs(recvline,stdout)==EOF)
		{
			printf("fputs error");
			return -1;
		}
	}
	printf("read num = %d\r\n",cnt);
	//if n==0,closed the connect normally;else error
	if(n < 0)
		printf("read error");
	return(0);	
}
