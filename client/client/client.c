//
//  client.c
//  socket2
//
//  Created by 丁贵强 on 16/3/13.
//  Copyright © 2016年 丁贵强. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define  MAXLINE 4096


int main(int argc, char ** argv){
    int sockfd;
    ssize_t n,rec_len;
    
    char recvline[MAXLINE],sendline[MAXLINE];
    char buf[MAXLINE];
    
    struct sockaddr_in servaddr;
    
    if (argc  != 2) {
        printf("使用：./client ip地址\n");
        exit(1);
    }
    
    if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1) {
        printf("创建socket失败 : %s 错误号： %d\n",strerror(errno),errno);
        exit(1);
    }
    
    // 服务器的地址
    memset(&servaddr, 0, sizeof(servaddr));
    
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8000);
    if ((inet_pton(AF_INET,argv[1],&servaddr.sin_addr)) <= 0) {
        printf("inet_pton 出错 for %s\n",argv[1]);
        exit(1);
    }
    
   //    servaddr.sin_addr
    // inet_addr()
    // 连接服务器
    if (connect(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        printf("连接服务器出错 %s 错误号 %d\n",strerror(errno),errno);
        exit(1);
    }
    
    
    printf("发送数据给服务器\n");
    fgets(sendline, MAXLINE, stdin);
    if ( send(sockfd,sendline,strlen(sendline),0) < 0 ) {
        printf("发送数据给服务器 出错 %s (错误号：%d)\n",strerror(errno),errno);
        exit(1);
    }
    
    if ((rec_len = recv(sockfd, buf,MAXLINE, 0))== -1) {
        perror("接收出错");
        exit(1);
    }
    buf[rec_len] = '\0';
    printf("接收到的数据为: %s\n",buf);
    close(sockfd);
    return 0;
}