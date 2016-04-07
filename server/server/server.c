//
//  server.c
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


#define DEFAULT_PORT 8000
#define MAXLINE 4096



int main(int argc, const char * argv[]) {
    
    int socket_fd, connect_fd;
    struct sockaddr_in servaddr; // 本机
    char buff[MAXLINE];
    ssize_t n;
    
    
    // 初始化socket
    if( (socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        printf("创建socket 失败: %s,错误号: %d\n",strerror(errno),errno);
        exit(0);
    }
    
    printf("创建成功的socket 是: %d\n",socket_fd);
    
    // 清0
    memset(&servaddr, 0, sizeof(servaddr));
    
    /**
     *  装配地址
     */
    servaddr.sin_family = AF_INET; // ipv4
    
    // 将IP地址设置成INADDR_ANY，让系统自动获取本机的IP地址
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);// 32位
    //   include <arpa/inet.h>
    // inet_addr(INADDR_ANY);
    //htonl(INADDR_ANY);
    // 设置端口号
    servaddr.sin_port = htons(DEFAULT_PORT);// 16位
    
    printf("装配好的地址信息是----网络协议：%u \t \
           地址： %d \t \
           端口号：%hd \n ",
           servaddr.sin_family,
         ntohl(servaddr.sin_addr.s_addr),
         ntohs(servaddr.sin_port)
           );
    
    
    // 将本地地址 绑定到所创建的socket上
    if (bind(socket_fd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
        printf("绑定socket 失败: %s,错误号: %d\n",strerror(errno),errno);
        exit(0);
    }
    
    
    // 开始监听是否有客户端连接
    if ((listen(socket_fd, 10)) == -1) { // 10代表什么意思？
        printf("监听socket 失败: %s,错误号: %d\n",strerror(errno),errno);
        exit(0);
    }
    
    printf("===== 等待客户端连接 =====\n");
    while (1) {
        // 阻塞直到有客户端连接，不然多浪费CPU资源
        if( (connect_fd = accept(socket_fd, (struct sockaddr *)NULL, NULL)) == -1)
        {
            printf(" 接受 socket 失败: %s,错误号: %d\n",strerror(errno),errno);
            continue;
        }
    
    
         // 接受客户端传过来的数据
        n = recv(connect_fd, buff, MAXLINE, 0);
    
        // 向客户端发送回应数据
        if (!fork())
        {
          // 子进程发送数据
            printf("长度为: %ld \n",sizeof("你好！你已经连上服务器了。\n"));
          if ((send(connect_fd, "你好！你已经连上服务器了。\n", sizeof("你好！你已经连上服务器了。\n"), 0) ) == -1 )
          {
             perror("服务器发送出错");
             close(connect_fd);
             exit(0);
           }
            
            //
            buff[n] = '\0';
            printf("从客户端发来的数据： %s\n",buff);
            close(connect_fd);
       }
    }
    
    close(socket_fd);
    return 0;
}



