/*************************************************************************
	> File Name: m_client.h
	> Author:FW1509 
	> Remarks: 客户端类
	> Created Time: Fri 20 May 2022 08:37:25 PM CST
 ************************************************************************/

#ifndef _M_CLIENT_H_
#define _M_CLIENT_H_

#include<unistd.h>
#include<sys/types.h>
#include<sys/epoll.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<memory>
#include<ctime>

#include<iostream>
#include<string>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

#include"../public/log.h"
#include"m_recv_node.h"
#include"m_send_node.h"

class m_client
{
public:
    using SOCKET = int;
    static constexpr SOCKET INVALID_SOCKET = (SOCKET)(~0);
    static constexpr int SOCKET_ERROR = -1;

public:
    m_client();
    virtual ~m_client();
    m_client(const m_client&) = delete;
    m_client& operator= (const m_client&) = delete;

    /* 
    * 环境初始化
    * 返回0为成功
    */
    int m_init();

    /*
    * 连接服务端
    * 返回0为成功
    */
    int m_connect(const char* ip, unsigned short port);

    //接收/发送线程开始工作
    void m_start();
    
    //开始工作
    void m_work();

    //关闭client
    void m_close();

    //进程exit
    void m_exit();

private:
    //本机socket
    SOCKET _sock;

    //连接状态
    bool _status;

    //阻塞信号量
    m_semaphore _sem;
    
    //报文结果
    int _login_ret;
    int _operate_ret;

    //接收节点
    m_recv_node* _rnode;
    
    //发送节点
    m_send_node* _snode;
};
#endif
