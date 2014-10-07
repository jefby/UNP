/*
	show the client's socket address 
	Author:jefby
	Email:jef19906@gmail.com
*/
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <time.h>

#define MAXLINE 4096
#define LISTENQ 1024
#define SA struct sockaddr_in

static char client_addr[MAXLINE];
int main(int argc,char**argv)
{
	//listenfd 倾听套接字描述符
	int listenfd,connfd;
	SA servaddr,cliaddr;
	char buff[MAXLINE];
	time_t ticks;
	int cli_len;
	int i = 0;
	//创建一个网际套接字
	listenfd = socket(AF_INET,SOCK_STREAM,0);
	if(listenfd < 0 ){
		printf("server socket function error\n");
		return -1;
	}
	//清空数据
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	//这里htonl是将32位的主机地址转换成网络地址
	//INADDR_ANY表示任意地址，或者不确定地址，若服务器有多个网络接口
	//则可以在任意接口上接收到客户连接，用于多块网卡监听
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	//htons将16位的主机地址转换成网络地址，时间端口号
	servaddr.sin_port=htons(9999);
	//将熟知端口绑定到套接字listenfd上
	bind(listenfd,(SA*)&servaddr,sizeof(servaddr));
	//将套接字转换成侦听套接字，LISTENQ定义了系统内核允许在这个监听
	//描述符的最大客户连接数
	listen(listenfd,LISTENQ);
	
	while(1)
	{
		//接受客户连接
		connfd=accept(listenfd,(SA*)&cliaddr,&cli_len);
		//将sin_addr转换成点分十进制字符串
		inet_ntop(AF_INET,&cliaddr.sin_addr,client_addr,MAXLINE);
		printf("client's address=%.100s:%d\r\n",client_addr,ntohs(cliaddr.sin_port));
		ticks=time(NULL);
		//最大输出24个字符
		snprintf(buff,sizeof(buff),"%.24s\r\n",ctime(&ticks));
		for(i=0;i<strlen(buff);++i)
			write(connfd,buff+i,1);
		close(connfd);
	}
		
}
