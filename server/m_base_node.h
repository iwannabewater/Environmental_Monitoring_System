/*************************************************************************
	> File Name: m_base_node.h
	> Author:FW1509 
	> Remarks: 服务器节点的基类 包含一个处理线程
	> Created Time: Fri 20 May 2022 04:38:11 PM CST
 ************************************************************************/

#ifndef _M_BASE_NODE_H_
#define _M_BASE_NODE_H_

#include<unistd.h>
#include<sys/types.h>
#include<sys/epoll.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#include<string.h>
#include<errno.h>

#include"../public/log.h"
#include"../public/m_thread.h"

class m_server;

class m_base_node
{
public:
    //启动此节点的工作线程
    virtual void start() = 0;
    //关闭此节点的工作线程
    virtual void close_node() = 0;

protected:
    /*
    * 三个线程事件 创建/工作/销毁
    * 传参为事件所属线程
    *
    *  func_create: func_run()开始前执行
    *     func_run: 线程工作函数
    * func_destory: func_run()结束后执行
    */
    virtual void func_create(m_thread* thread) = 0;
    virtual void func_run(m_thread* thread) = 0;
    virtual void func_destory(m_thread* thread) = 0;

protected:
    //节点工作线程
    m_thread _thread;

};

#endif
