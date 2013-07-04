/*
	Author:jefby
	Email:jef19906@gmail.com
*/
#include <stdio.h>
#include <netinet/in.h>
#include <time.h>
#define MAXLINE 100
#define LISTENQ 1024
#define SA struct sockaddr_in

int main(int argc,char**argv)
{
	//listenfd 倾听套接字描述符
	int listenfd,connfd;
	struct sockaddr_in servaddr;
	char buff[MAXLINE];
	time_t ticks;
	//创建一个网际套接字
	listenfd = socket(AF_INET,SOCK_STREAM,0);
	//清空数据
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	//这里htonl是将32位的主机地址转换成网络地址
	//INADDR_ANY表示任意地址，或者不确定地址，若服务器有多个网络接口
	//则可以在任意接口上接收到客户连接
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	//htons将16位的主机地址转换成网络地址
	servaddr.sin_port=htons(13);
	//将熟知端口绑定到套接字listenfd上
	bind(listenfd,(SA*)&servaddr,sizeof(servaddr));
	//将套接字转换成真听套接字，LISTENQ定义了系统内核允许在这个监听
	//描述符的最大客户连接数
	listen(listenfd,LISTENQ);
	while(1)
	{
		//接受客户连接
		connfd=accept(listenfd,(SA*)NULL,NULL);
		ticks=time(NULL);
		//最大输出24个字符
		snprintf(buff,sizeof(buff),"%.24s\r\n",ctime(&ticks));
		write(connfd,buff,strlen(buff));
		close(connfd);
	}
		
}
