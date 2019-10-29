#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
//#include <stdbool.h>

#define bool int
#define true 1
#define false 0

#define MYPORT 8888
#define BACKLOG 5    //how many pending connections queue will hold
#define BUF_SIZE 200
int fd_A[BACKLOG];    //accepted connection fd
int conn_amount;      //current connection amount
void showclient()
{
	printf("client amount: %d\n", conn_amount);
}

int main()
{
	int sock_fd, new_fd;  //listen on sock_fd, new connection on new_fd
	struct sockaddr_in server_addr, client_addr;
	socklen_t sin_size;
	int yes = 1, ret, i;
	char buf[BUF_SIZE];
	fd_set fdsr;  //数组宏定义,与文件句柄产生联系
	int maxsock;
	struct timeval tv;
	if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)   //创建socket
	{
		perror("socket create error");
		exit(1);
	}

	if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)  //设置socket的属性，socket关闭后立即收回 用以加强程序的健壮性
	{
		perror("setsockopt error!");
		exit(1);
	}
	server_addr.sin_family = AF_INET;   //地址簇
	server_addr.sin_port = htons(MYPORT);
	server_addr.sin_addr.s_addr = INADDR_ANY;    //自动填充为本机IP
	memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));

	if ( bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr))== -1)   //绑定
	{
		perror("bind fail!\n");
		exit(1);
	}
	if (listen(sock_fd, BACKLOG))  //开始监听
	{
		perror("listen fail!\n");
		exit(1);
	}
	printf("listen port %d\n", MYPORT);  //监听成功
	conn_amount = 0;
	sin_size = sizeof(client_addr);
	maxsock = sock_fd;

	while (true)
	{
		//initialize file descript set 
		FD_ZERO(&fdsr);
		FD_SET(sock_fd, &fdsr);
		tv.tv_sec = 30;
		tv.tv_usec = 0;
		//加入活跃的连接
		for (i = 0; i < BACKLOG; i++)
		{
			if (fd_A[i] != 0)
			{
				FD_SET(fd_A[i], &fdsr);
			}
		}

		ret = select(maxsock + 1, &fdsr, NULL, NULL, &tv);
		if (ret < 0)
		{
			perror("select error!\n");
			break;   //退出循环
		}
		else if (ret == 0)
		{
			printf("select timeout!\n");
			continue;      //继续循环
		}
		//检查集合内的每个socket连接
		for (i = 0; i < conn_amount; i++)
		{
			if (FD_ISSET(fd_A[i], &fdsr))
			{
				ret = recv(fd_A[i], buf, sizeof(buf), 0);
				if (ret <= 0)             //客户端关闭
				{
					printf("client[%d] close\n", i);
					close(fd_A[i]);
					FD_CLR(fd_A[i], &fdsr);    //将该文件描述符从文件描述符集中清除
					fd_A[i] = 0;
				}
				else     //输出接收到的数据
				{
					if (ret < BUF_SIZE)
					{
						memset(&buf[ret], '\0', 1);
					}
					printf("client[%d] send:%s\n", i, buf);
				}
			}
		}

		//检查是否有新连接接入
		if (FD_ISSET(sock_fd, &fdsr))
		{
			new_fd = accept(sock_fd, (struct sockaddr *)&client_addr, &sin_size);
			if (new_fd <= 0)
			{
				perror("accept error!");
				continue;
			}
			//增加至队列
			if (conn_amount < BACKLOG)
			{
				fd_A[conn_amount++] = new_fd;
				printf("new connection client[%d] %s:%d\n", conn_amount, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
				if (new_fd > maxsock)
				{
					maxsock = new_fd;
				}
			}
			else
			{
				printf("已达到最大连接数，退出\n");
				send(new_fd, "bye", 4, 0);
				close(new_fd);
				break;
			}
		}
		showclient();
	}

	//关闭其他连接
	for (i = 0; i < BACKLOG; i++)
	{
		if (fd_A[i] != 0)
			close(fd_A[i]);
	}
	exit(0);
}
