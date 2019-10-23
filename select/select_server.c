#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>

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

}
