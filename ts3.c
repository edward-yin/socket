#include<netinet/in.h>                         // for sockaddr_in  
#include<sys/types.h>                          // for socket  
#include<sys/socket.h>                         // for socket  
#include<stdio.h>                              // for printf  
#include<stdlib.h>                             // for exit  
#include<string.h>                             // for bzero  
#include <errno.h>

#define BUF_SIZE 1024

int main()
{
	struct sockaddr_in serv_addr;
	bzero(&serv_addr,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htons(INADDR_ANY);
	serv_addr.sin_port = htons(1234);
	
	int serv_sock = socket(AF_INET,SOCK_STREAM,0);
	if(serv_sock < 0){
		printf("creat serv_sock failed\n");
		exit(0);
	}
	
	if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))){
		printf("bind socket error:%s(errno:%d)\n",strerror(errno),errno);
        exit(0);
	}
	
	if(listen(serv_sock,20)){
		printf("listen socket error:%s(errno:%d)\n",strerror(errno),errno);
        exit(0);
	}
	
	printf("====waiting for client's request====\n");
	
	char *filename = "test.txt";
	char buffer[BUF_SIZE];
	FILE *fp = fopen(filename,"r");
	struct sockaddr_in clnt_addr;
	socklen_t socklen = sizeof(clnt_addr);
	int clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_addr,&socklen);
	if (clnt_sock<0){
		printf("accept error: %s(errno:%d)\n",strerror(errno),errno);
		exit(0);
	}
	

	if (fp == NULL){
		printf("open file error: %s(errno:%d)\n",strerror(errno),errno);
	}
	else {

		bzero(buffer,BUF_SIZE);
		int filel = 0;
		
		while(filel = fread(buffer,1,BUF_SIZE,fp)>0){
			printf("file length = %d\n",filel);
			
			if (send(clnt_sock,buffer,filel,0) < 0){
				printf("send error\n : %s(errno:%d)\n",strerror(errno),errno);
				exit(0);
			}
			bzero(buffer,sizeof(buffer));
		}
		fclose(fp);
		printf("successs!\n");
	}
	
	close(clnt_sock);
	close(serv_sock);
	
	return 0;
	
}

