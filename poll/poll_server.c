#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>

#define MAXLINE 10240
#define MYPORT 6888
#define BACKLOG 3
#ifndef OPEN_MAX
#define OPEN_MAX 40960
#endif

void handle(struct pollfd* clients, int maxClient, int readyClient);

int main(int argc, char* argv[]) {
	int listenfd, connfd;   //监听，连接文件描述符
	int set_sock = 1;  //调用close之后，socket立即关闭
	struct pollfd clients[OPEN_MAX];  
	socklen_t socklen = sizeof(struct sockaddr_in);
	struct sockaddr_in client_addr, server_addr;      //地址结构
	char buf[MAXLINE];
	int nready;    //事件发生的pollfd结构的个数
	memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(MYPORT);   //主机序列转换为网络序列

	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket error");
	}
	if (setsockopt(listen, SOL_SOCKET, SO_REUSEADDR, &set_sock, sizeof(int)) < 0) {
		perror("seetsocket error");
	}

	if (bind(listen, (struct sockaddr *)&server_addr, socklen) < 0) {
		perror("bind error");
		exit(-1);
	}

	if (listen(listenfd, BACKLOG) <0)
	{
		perror("listen error");
		exit(-1);
	}

	//开始poll处理...
	return 0;
}
