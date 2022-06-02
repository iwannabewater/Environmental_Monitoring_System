/*************************************************************************
	> File Name: m_group_node.h
	> Author:FW1509 
	> Remarks: 服务器组节点 处理以组为单位的用户报文
	> Created Time: Fri 20 May 2022 07:54:57 PM CST
 ************************************************************************/

#ifndef _M_GROUP_NODE_H_
#define _M_GROUP_NODE_H_

#include<string.h>
#include<vector>
#include<map>
#include<mutex>
#include"m_client_node.h"
#include"../public/m_timer.h"
#include"../public/m_datagram.h"
#include"m_base_node.h"
#include"m_task_node.h"

class m_group_node : protected m_base_node
{
public:
    using SOCKET = int;
    static constexpr int GROUP_NAME_MAX_SIZE = 50;
    static constexpr int EVENT_MAX_SIZE = 100;
    static constexpr int RECV_BUF_SIZE = 4096;

public:
    m_group_node(int g_id, m_server* server);
    virtual ~m_group_node();
    m_group_node() = delete;
    m_group_node(const m_group_node&) = delete;
    m_group_node& operator= (const m_group_node&) = delete;
    int get_client_num() const { return _client_num; }

    /*
    * 添加新的监听socket
    */
    void addclient(m_client_node* client);

protected:
    virtual void start() override;
    virtual void close_node() override;
    virtual void func_create(m_thread* thread) override;
    virtual void func_run(m_thread* thread) override;
    virtual void func_destory(m_thread* thread) override;

private:
    /*
    * 处理客户端消息
    * 返回-1为客户端退出/收到非法包
    */
    int recvdata(SOCKET sockfd);
    
    /*
    * 发送s2c心跳包
    */
    void send_s2c_heart(SOCKET sockfd);

    /*
    * 发送data到pc
    */
    void send_data_to_pc(SOCKET sockfd);

private:
    //所属server
    m_server* _server;
    int _group_id;

    //锁
    std::mutex _mutex;
    //监听队列及其缓冲区
    std::vector<m_client_node*> _clientbuf;
    std::unordered_map<SOCKET, m_client_node*> _clients;
    //当前监听用户数量
    int _client_num;

    //recv_buf
    char* _recv_buf;
    
    //所属task节点
    m_task_node _tnode;
};

#endif
