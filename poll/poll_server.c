#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <arpa/inet.h>

#define MAXLINE 10240
#define MYPORT 6888
#define BACKLOG 3

void handle(struct pollfd* clients);

int main(int argc, char* argv[]) {
	//1、创建套接字及绑定ip，端口
	int listenfd, connfd;   //监听，连接文件描述符
	int set_sock = 1; 
	int i,maxpoll = 0;
	struct pollfd clients[BACKLOG];
	for (i = 1; i < BACKLOG; i++)
	{
		clients[i].fd = -1;
	}//初始化数组
	socklen_t socklen = sizeof(struct sockaddr_in);
	struct sockaddr_in client_addr, server_addr;      
	char buf[MAXLINE];
	memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(MYPORT);   //主机序列转换为网络序列
	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket error");
	}
	//2、设置端口复用
	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &set_sock, sizeof(int)) < 0) {
		perror("seetsocket error");
	}
	if (bind(listenfd, (struct sockaddr *)&server_addr, socklen) < 0) {
		perror("bind error");
		exit(-1);
	}
	//3、设置监听的上限数
	if (listen(listenfd, BACKLOG) <0)
	{
		perror("listen error");
		exit(-1);
	}

	//4、将监听的文件描述符加入poll监听的数组中
	clients[0].fd = listenfd;
	clients[0].events = POLLIN;    

	int nready;
	printf("poll调用开始，监听端口:%d\n", MYPORT);
	printf("最大的连接数为:%d\n", BACKLOG);

	while (1)
	{
		nready = poll(clients, BACKLOG, -1);   //无限等待
		printf("nready:%d\n", nready);
		if (nready == -1)
		{
			perror("poll error");
		}
		printf("revents:%d\n", clients[0].revents);
		if (clients[0].revents & POLLIN)
		{
			connfd = accept(listenfd, (struct sockaddr *)&client_addr, &socklen);
			printf("接收到地址为:%d,端口为:%d的连接\n",inet_ntoa(client_addr.sin_addr), client_addr.sin_port);
			for (i = 1; i < BACKLOG; i++)
			{
				if (clients[i].fd == -1)
				{
					clients[i].fd = connfd;
					clients[i].events = POLLIN;
					if (i > maxpoll)
					{
						maxpoll = i;
					}
					break;
				}
			}
			if (i == BACKLOG)
			{
				printf("已达到最大连接数，关闭该连接\n");
				close(connfd);
			}
		}
		handle(clients);
	}
	return 0;
}

//处理客户端请求
void handle(struct pollfd * clients) {
	printf("handle clients;\n");
	int i, nread;
	int connfd;
	char buf[MAXLINE];
	for (i = 1; i < BACKLOG; i++)
	{
		if (clients[i].fd == -1)
			continue;
		connfd = clients[i].fd;
		printf("clients[%d].revents:%d\n",i,clients[i].revents);
		//判断事件是否发生
		if (clients[i].revents & (POLLIN |POLLERR)){
			nread = recv(connfd, buf, sizeof(buf), 0);
			if (nread <= 0){
				perror("recv error!\n");
				close(connfd);
				clients[i].fd = -1;
				continue;
			}
			else{
				buf[nread] = '\0';
				printf("client:%s\n", buf);
				continue;
			}
			

		}
	}
}
