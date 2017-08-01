#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUF_SIZE 1024

int main(int argc,char *argv[])
{
	int serv_sock,clnt_sock,n;
	char buffer[BUF_SIZE];
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	
	socklen_t clnt_addr_size = sizeof(clnt_addr);
	//create
	serv_sock = socket(AF_INET,SOCK_STREAM,0);
	clnt_sock = socket(AF_INET,SOCK_STREAM,0);
	if ((serv_sock == -1)||(clnt_sock == -1)){
		printf("create socket error: %s(errno:%d)\n",strerror(errno),errno);
		exit(0);
	}
	

	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(1234);
	
	//bind 
	if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) == -1){
		printf("bind socket error:%s(errno:%d)\n",strerror(errno),errno);
		exit(0);
	}
	
	
	
	//listening
	if(listen(serv_sock,20) == -1){
		printf("listen socket error:%s(errno:%d)\n",strerror(errno),errno);
		exit(0);
	}
	
	
	printf("====waiting for client's request====\n");
	
	char *str = "client query";	
	int c;
	while(1)
	{
		bzero(&buffer,sizeof(buffer));
		//accept request from client
		if(clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_size) == -1){
			printf("accept socket error:%s(errno:%d)\n",strerror(errno),errno);
			continue;
		}
		
		n = recv(clnt_sock,buffer,BUF_SIZE,0);
		buffer[n] = '\0';
		/*c = strncasecmp(buffer,str,11);
		//printf("%d\n",c);
		if (c == 0){
			printf("recv client query\n");
		}
*/
		printf("recv msg from client:%s\n",buffer);
		if (write(clnt_sock,buffer,sizeof(buffer)) < 0){
                        printf("send msg error: %s(errno:%d)\n",strerror(errno),errno);
                        exit(0);

                }

		close(clnt_sock);
	}

	close(serv_sock);
	
}
