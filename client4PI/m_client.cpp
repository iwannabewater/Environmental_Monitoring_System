/*************************************************************************
	> File Name: m_client.cpp
	> Author:FW1509 
	> Remarks: 客户端类
	> Created Time: Fri 20 May 2022 08:37:25 PM CST
 ************************************************************************/

#include"m_client.h"

m_client::m_client():
    _sock(INVALID_SOCKET),
    _status(false),
    _login_ret(-1),
    _operate_ret(-1),
    _rnode(nullptr)
{
    _rnode = new m_recv_node(this);
    _snode = new m_send_node(this);
}

m_client::~m_client()
{
    DEBUG("Client ~m_client start");
    m_close();
    
    delete _rnode;
    delete _snode;
    close(_sock);
    
    DEBUG("Client ~m_client end");
}

int 
m_client::m_init()
{
    //创建socket
    if(_sock != INVALID_SOCKET)//存在socket
    {
        DEBUG("Client init warn -- socket:%d already exists", _sock);
        m_close();
    }
    _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(_sock == INVALID_SOCKET)
    {
        DEBUG("Client init faild -- %s", strerror(errno));
        return -1;
    }

    DEBUG("Client init(%d) success", _sock);
    return 0;
}

int 
m_client::m_connect(const char* ip, unsigned short port)
{
    //无效套接字
    if(_sock == INVALID_SOCKET)
    {
        DEBUG("Client bind warn -- client socket not initialized");
        m_init();
    }

    //配置
    sockaddr_in _sin = {};
    _sin.sin_family = AF_INET;//IPV4
    _sin.sin_port = htons(port);//端口
    _sin.sin_addr.s_addr = inet_addr(ip);//IP

    //connect
    int ret = connect(_sock, (sockaddr*)&_sin, sizeof(sockaddr_in));
    if(ret == SOCKET_ERROR)
    {
        DEBUG("Client connect(%d) ip:%s port:%d faild -- %s", _sock, ip, port, strerror(errno));
        close(_sock);
        return -1;
    }
    
    DEBUG("Client connect(%d) success", _sock);
    _status = true;
    return 0; 
}

void 
m_client::m_start()
{
    DEBUG("Client m_start start");
    _rnode->set_sockfd(_sock);
    _rnode->start();
    _snode->set_sockfd(_sock);
    _snode->start();
}

void
m_client::m_work()
{
    if(_status == false)
    {
        printf("connect... failed\n");
        return;
    }
    printf("connect... success\n");
   
    _sem.wait();
}

void 
m_client::m_close()
{
    DEBUG("Client m_close start");
    _rnode->close_node();
    _snode->close_node();
}

void
m_client::m_exit()
{
    printf("\n-------------------------------------"
           "\nNetwork error: disconnect from server"
           "\n-------------------------------------\n");
    exit(1);
}

