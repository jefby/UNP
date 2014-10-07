#include <stdio.h>

#define MAXLINE 4096

static int read_cnt;
static char * read_ptr;
static char read_buf[MAXLINE];
//主要是自己管理缓冲区,使用MAXLINE大小的缓冲区,一次读取这样大小的数据,然后每次从缓冲区中取得数据
static ssize_t my_read(int fd,char *ptr)
{
	if(read_cnt <= 0)
	{
		again:
		if((read_cnt=read(fd,read_buf,sizeof(read_buf))) < 0)
		{
			if(errno == EINTR) //重新调用read函数
				goto again;//stall or interrupt by some signal
			return -1;
		}
		else if(read_cnt == 0)//EOF
			return 0;
		read_ptr = read_buf;
	}
	read_cnt--;
	*ptr = *read_ptr++;
	return 1;
}

ssize_t readline(int fd,void *vptr,size_t maxlen)
{
	ssize_t n,rc;
	char c,*ptr;
	ptr = vptr;
	for(n=1;n<maxlen;n++)
	{
		if((rc=my_read(fd,&c))==1)
		{
			*ptr++=c;
			if(c=='\n')
				break;//newline is stored,like fgets()
		}
		else if(rc == 0)
		{
			*ptr = 0;
			return n-1;//EOF,n-1 bytes were read
		}
		else
			return (-1);//error,errno set by read()
	}
	*ptr = 0;
	return n;
}
ssize_t readlinebuf(void **vptrptr)
{
	if(read_cnt)
		*vptrptr = readptr;
	return (read_cnt);
}

int main(int argc,char **argv)
{

	return 0;
}
