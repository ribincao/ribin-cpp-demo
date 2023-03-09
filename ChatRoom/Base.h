#ifndef IM_BASE_H
#define IM_BASE_H

#include <iostream>
#include <list>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 4000
#define EPOLL_SIZE 5000
#define BUF_SIZE 0xFFFF
#define WELCOME "Welcome to chat room. Your id is: #%d"
#define MESSAGE "#%d: %s"
#define PRIVATE_MESSAGE "#%d say to you: %s"
#define PRIVATE_ERROR "#%d is not in chat room yet."
#define EXIT "@EXIT"
#define SINGLE "Only you in the chat room."
#define ET true
#define CONTENT_SIZE 0xFFF

//  定义消息结构体
struct Msg
{
    int type;   //  类型, 0 - 群聊, 1 - 私聊
    int from;   //  发送者 ID
    int to;     //  接收者 ID
    char content[CONTENT_SIZE];
};

//  给 epollfd 添加事件
static void addfd(int epollfd, int fd, bool et_flag)
{
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN;
    //  设置边缘触发
    if(et_flag) ev.events |= EPOLLET;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
    //  设置为 O_NONBLOCK
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFD, 0) | O_NONBLOCK);
    // std::cout << "fd added to epoll" << std::endl;
}
#endif