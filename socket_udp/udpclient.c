#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char ** argv)
{
	int sock, len, addr_len;
	char buf[128],buf2[128];
	struct sockaddr_in s_addr;       //服务端地址
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		perror("socket create failed.\n");
		exit(1);
	}
	else
	{
		printf("socket create successful.\n");
	}
	//地址赋值
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(8888);
	memset(s_addr.sin_zero, '\0', sizeof(s_addr.sin_zero));    //填充0目的使sizeof(sockaddr_in)==sizeof(sockaddr)
	if (argc > 2)
	{
		s_addr.sin_addr.s_addr = inet_addr(argv[1]);
		strcpy(buf, argv[2]);
	}
	else
	{
		printf("input server ip and parameter!\n");
		exit(0);
	}
	printf("parameter:%s\n", argv[1]);
	addr_len = sizeof(s_addr);
	len = sendto(sock, buf, strlen(buf), 0, (struct sockaddr *)&s_addr, addr_len);
	if (len < 0)
	{
		perror("send error!\n");
		exit(1);
	}
	sleep(1);
	printf("after sleep!\n");

	len = recvfrom(sock, buf2, sizeof(buf2) - 1, 0, (struct sockaddr *)&s_addr, &addr_len);

	printf("after recvfrom,len = %d!\n",len);

	if (len < 0)
	{
		perror("receive error!\n");
		exit(1);
	}
	buf2[len] = '\0';
	printf("receive from server :%s\n", buf2);
	close(sock);
	return 0;
}
