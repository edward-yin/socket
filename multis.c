#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>
#include<signal.h>

#define BUF_SIZE 1024


void progress(int sock)
{
	char buffer[BUF_SIZE];
	while(1)
	{
		bzero(buffer,BUF_SIZE);
		int rc = read(sock,buffer,BUF_SIZE);
		
		if(rc == -1){
			printf("read socket error: %s(errno:%d)\n",strerror(errno),errno);
			exit(0);
		}else if(rc == 0){
			printf("client close\n");
			break;
		}
		
		fputs(buffer,stdout);
		write(sock,buffer,rc);
		sleep(100);
	}
}


int main()
{
	signal(SIGCHLD,SIG_IGN);
	int serv_sock;
	if((serv_sock = socket(AF_INET, SOCK_STREAM,0)) == -1)
	{
		printf("create socket error: %s(errno:%d)\n",strerror(errno),errno);
		exit(0);
	}
	
	struct sockaddr_in servaddr;
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(1234);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	int on =1;
	if(setsockopt(serv_sock,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)) == -1)
	{
		printf("set socket option error: %s(errno:%d)\n",strerror(errno),errno);
		exit(0);
	}
	
	if(bind(serv_sock,(struct sockaddr *)&servaddr,sizeof(servaddr)) == -1)
	{
		printf("bind socket error: %s(errno:%d)\n",strerror(errno),errno);
		exit(0);
	}
	
	if(listen(serv_sock,20) == -1)
	{
		printf("listen socket error: %s(errno:%d)\n",strerror(errno),errno);
		exit(0);
	}
	
	printf("====waiting for client's request====\n");
	
	struct sockaddr_in clnt_addr;
	socklen_t socklen = sizeof(clnt_addr);

	
	pid_t pid;
	
	while(1)
	{
		int clnt_sock = accept(serv_sock,(struct sockaddr *)&clnt_addr,&socklen);	
		if(clnt_sock == -1)
		{
			printf("accept socket error: %s(errno:%d)\n",strerror(errno),errno);
			exit(0);
		}
		printf("recv socket from %s:%d\n",inet_ntoa(clnt_addr.sin_addr),
				ntohs(clnt_addr.sin_port));
		
		pid = fork();
		
		if(pid == -1){
			printf("fork error: %s(errno:%d)\n",strerror(errno),errno);
			exit(0);
		}else if (pid == 0){
			close(serv_sock);
			progress(clnt_sock);
		}else{
			close(clnt_sock);
		}

	}
	return 0;
}
