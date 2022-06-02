/*************************************************************************
	> File Name: m_send_node.h
	> Author:FW1509 
    > Remarks: 客户端send节点 向服务端send
	> Created Time: Fri 20 May 2022 08:37:25 PM CST
 ************************************************************************/

#ifndef _M_SEND_NODE_H_
#define _M_SEND_NODE_H_

#include<list>
#include<string>
#include<string.h>
#include<regex>
#include<fstream>

#include"../public/m_timer.h"
#include"../public/m_datagram.h"
#include"m_base_node.h"

class m_client;

class m_send_node : public m_base_node
{
public:
    using SOCKET = int;
    using task = std::function<void()>;

public:
    m_send_node(m_client* client);
    virtual ~m_send_node();
    m_send_node() = delete;
    m_send_node(const m_send_node&) = delete;
    m_send_node& operator= (const m_send_node&) = delete;
    
    //设置本机socket
    void set_sockfd(SOCKET sockfd) { _sockfd = sockfd; }
    
    /*
    * 启动与关闭登录节点
    */
    virtual void start() override;
    virtual void close_node() override;

protected:
    virtual void func_create(m_thread* thread) override;
    virtual void func_run(m_thread* thread) override;
    virtual void func_destory(m_thread* thread) override;

private:
    /*
    * 添加任务
    */
    void addtask(task t);

    //传输数据
    void send_data_to_server();

    //获取光照数据
    void get_illu_from_server();

private:
    //所属client
    m_client* _client;
    
    //本机socket
    SOCKET _sockfd;
    
    /*
    * task缓冲区相关
    */
    std::list<task> _tasks;
    std::list<task> _tasksbuf;
    std::mutex _mutex;
};

#endif
