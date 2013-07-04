/*
	UNP chapter 1 example : request to the server for current time
	Author:jefby
	Email:jef199006@gmail.com
*/
#include "unp.h"
//#include "apueerror.h"
int main(int argc,char **argv)
{
	//sockfd:套接字描述符
	int sockfd,n;
	//接收缓冲
	char recvline[MAXLINE+1];
	//套接字IPV4
	struct sockaddr_in	servaddr;
	
	if(argc != 2)
		err_quit("usage: a.out <IP address>");
	//创建一个网际字节流
	if((sockfd=socket(AF_INET,SOCK_STREAM,0)) < 0)
		err_sys("socket error");
	//初始化为0
	bzero(&servaddr,sizeof(servaddr));
	
	servaddr.sin_family = AF_INET;
	//设置服务器端口；获取时间为熟知端口13
	servaddr.sin_port = htons(13);
	//将命令行中的IP地址转换为数值
	if(inet_pton(AF_INET,argv[1],&servaddr.sin_addr) <= 0)
		err_quit("inet_pton error for %s",argv[1]);
	//客户端向服务器发出请求，建立连接	
	if(connect(sockfd,(SA*)&servaddr,sizeof(servaddr)) <0)
		err_sys("connect error");
	//接收服务器传回的数据
	while((n=read(sockfd,recvline,MAXLINE))>0)
	{
		recvline[n]=0;	//null terminate
		if(fputs(recvline,stdout)==EOF)
			err_sys("fputs error");
	}
	//if n==0,closed the connect normally;else error
	if(n < 0)
		err_sys("read error");
	exit(0);	
}
