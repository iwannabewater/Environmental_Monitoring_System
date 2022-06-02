/*************************************************************************
	> File Name: m_client_node.cpp
	> Author:FW1509 
	> Remarks: 客户端节点 一个实例对应一个客户端连接
	> Created Time: Fri 20 May 2022 04:19:41 PM CST
 ************************************************************************/

#include"m_client_node.h"

m_client_node::m_client_node(SOCKET sockfd):
    _sockfd(sockfd),
    _msg_recv_buf(nullptr),
    _len_recv_buf(0),
    _hb_time(0)
{
    //缓冲
    _msg_recv_buf = new char[CLIENT_RECV_BUF_SIZE];
    _len_recv_buf = 0;
}

m_client_node::~m_client_node()
{
    INFO("client_node socket(%d) quit", _sockfd);
    delete[] _msg_recv_buf;
    close(_sockfd);
}

bool 
m_client_node::check_hb(double time_interval)
{
    _hb_time += time_interval;
    if(_hb_time > CLIENT_HREAT_TIME_LOGINED)
    {
        INFO("check_hb dead socket(%d) time(%lf)", _sockfd, _hb_time);
        return true;
    }
    return false;
}

