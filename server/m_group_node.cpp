/*************************************************************************
	> File Name: m_group_node.cpp
	> Author:FW1509 
	> Remarks: 服务器组节点 处理以组为单位的用户报文
	> Created Time: Fri 20 May 2022 07:54:57 PM CST
 ************************************************************************/

#include"m_server.h"
#include"m_group_node.h"

m_group_node::m_group_node(int g_id, m_server* server):
    _server(server),
    _group_id(g_id),
    _client_num(0),
    _recv_buf(nullptr)
{
    //recv_buf初始化
    _recv_buf = new char[RECV_BUF_SIZE];

    _tnode.id = g_id;
}

m_group_node::~m_group_node()
{
    DEBUG("group_node ~group_node() start");
    //关线程
    close_node();
    
    //释放客户端
    for(m_client_node* c : _clientbuf)
    {
        delete c;
    }
    for(auto& [s, c] : _clients)
    {
        delete c;
    }
    _clientbuf.clear();
    _clients.clear();
   
    delete[] _recv_buf;
    DEBUG("group_node ~group_node() end");
}

void 
m_group_node::addclient(m_client_node* client)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _clientbuf.push_back(client);
    
    //有客户端节点时需保证工作线程运行
    ++_client_num;
    if(_thread.is_run() == false)
    {
        this->start();
    }
}

void
m_group_node::start()
{
    INFO("group_node thread start");
    _thread.start(
        //create
        [this](m_thread*)
        {
            func_create(&_thread);
        },
        //run
        [this](m_thread*)
        {
            func_run(&_thread);
        },
        //destory
        [this](m_thread*)
        {
            func_destory(&_thread);
        }
    );
    //tnode
    _tnode.start();
}

void
m_group_node::close_node()
{
    INFO("group_node thread close");
    _thread.close();
    _tnode.close_node();
}

void 
m_group_node::func_create(m_thread* thread)
{
    DEBUG("group_node func_create() start"); 
}

void 
m_group_node::func_run(m_thread* thread)
{
    DEBUG("group_node func_run() start");
    //组节点epoll关注登录用户的请求事件
    int epollfd = epoll_create(1);
    struct epoll_event ev;
    struct epoll_event events[EVENT_MAX_SIZE];
    int event_count;

    //计时器->heartbeat
    m_timer timer;
    timer.update();

    while(_thread.is_run())
    {
        //缓冲区socket加入epoll监听队列
        if(!_clientbuf.empty())
        {
            std::lock_guard<std::mutex> lock(_mutex);
            for(m_client_node* client : _clientbuf)
            {
                //epoll监听队列
                memset(&ev, 0, sizeof(struct epoll_event));
                ev.data.fd = client->get_sockfd();
                ev.events = EPOLLIN;
                epoll_ctl(epollfd, EPOLL_CTL_ADD, client->get_sockfd(), &ev);
                //加入正式队列
                _clients[client->get_sockfd()] = client;
            }
            _clientbuf.clear();
        }
        
        //监听登录事件
        event_count = epoll_wait(epollfd, events, EVENT_MAX_SIZE, 1);
        if(event_count < 0)
        {
            ERROR("group_node epoll_wait() error -- %s", strerror(errno));
            _thread.exit();
            break;
        }
        //存在则处理事件
        if(event_count > 0)
        {
            for(int i = 0; i < event_count; ++i)
            {
                if(recvdata(events[i].data.fd) == -1)
                { 
                    std::lock_guard<std::mutex> lock(_mutex);
                    //删除epoll事件监听
                    memset(&ev, 0, sizeof(struct epoll_event));
                    ev.data.fd = events[i].data.fd;
                    ev.events = EPOLLIN;
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, events[i].data.fd, &ev);
                    
                    //释放客户端节点并移出队列
                    delete _clients[events[i].data.fd];
                    _clients.erase(events[i].data.fd);
                    
                    //更新计数
                    --_client_num;
                }
            }
        }
        
        //处理心跳检测
        double time_interval = timer.get_sec();
        timer.update();//更新时间
        for(auto iter = _clients.begin(); iter != _clients.end();)
        {
            //检测心跳是否超时
            if(iter->second->check_hb(time_interval) == true)
            {
                std::lock_guard<std::mutex> lock(_mutex);
                //删除epoll事件监听
                memset(&ev, 0, sizeof(struct epoll_event));
                ev.data.fd = iter->first;
                ev.events = EPOLLIN;
                epoll_ctl(epollfd, EPOLL_CTL_DEL, iter->first, &ev);
                
                //移出队列
                delete iter->second;
                _clients.erase(iter++);//避免迭代器失效
                
                //更新计数
                --_client_num;
                continue;
            }
            iter++;
        }
        
        //当不存在监听客户端节点时 则关闭工作线程
        if(_client_num == 0)
        {
            INFO("group_node thread exit", _group_id);
            _thread.exit();
            _tnode.close_node();
        }
    }

    close(epollfd);
}

void 
m_group_node::func_destory(m_thread* thread)
{
    DEBUG("group_node(%d) func_destory() start", _group_id);
}

int 
m_group_node::recvdata(SOCKET sockfd)
{
    m_client_node* client = _clients[sockfd];
    int buf_len = recv(sockfd, _recv_buf, RECV_BUF_SIZE, 0);

    //退出
    if(buf_len <= 0)
    {
        return -1;
    }
    
    //内容转至二级缓冲
    memcpy(client->get_recvbuf() + client->get_recvlen(), _recv_buf, buf_len);
    client->set_recvlen(client->get_recvlen() + buf_len);
    
    //处理
    while(client->get_recvlen() >= (int)sizeof(header))
    {
        //不完整包
        header* ph = (header*)client->get_recvbuf();
        if(client->get_recvlen() < ph->length)
        {
            return 0;
        }
        //DEBUG("new data: cmd:%d length:%d", ph->cmd, ph->length);
        
        //处理
        switch(ph->cmd)
        {
            //心跳包
            case CMD_C2S_HEART:
            {
                //send s2c_heart
                _tnode.addtask([this, sockfd]()
                {
                    this->send_s2c_heart(sockfd);
                });
                
                //心跳重置
                client->reset_hb();
            }
            break;
            
            //树莓派->server
            case CMD_C2S_DATA:
            {
                c2s_data* cd = (c2s_data*)ph;
                DEBUG("|收到数据| Id: %d Temp: %d Rh: %d dB: %d", cd->id, cd->Temp, cd->Rh, cd->Db);
                _server->setdata_tr(cd->Temp, cd->Rh, cd->Db);
                //send result
                _tnode.addtask([this, sockfd]()
                {
                    c2s_data_result ret;
                    ret.result = 1;
                    send(sockfd, (const char*)&ret, sizeof(ret), 0);
                });
            }
            break;

            //树莓派->server 获取光照
            case CMD_C2S_GET_ILLU:
            {
                DEBUG("树莓派端请求光照数据");
                //send result
                _tnode.addtask([this, sockfd]()
                {
                    s2c_get_illu_result ret;
                    auto pd = _server->getdata();
                    ret.illu = pd.illu;
                    send(sockfd, (const char*)&ret, sizeof(ret), 0);
                });
            }
            break;

            //PC->server 获取数据
            case CMD_C2S_GET_DATA:
            {
                DEBUG("PC端请求数据");
                //send result
                _tnode.addtask([this, sockfd]()
                {
                    this->send_data_to_pc(sockfd);
                });
                //心跳重置
                client->reset_hb();
            }
            break;

            //PC->server 设置光照
            case CMD_C2S_SET_ILLU:
            {
                c2s_set_illu* csi = (c2s_set_illu*)ph;
                DEBUG("|设置光照| illu: %d", csi->illu);
                _server->setdata_i(csi->illu);
                //send result
                _tnode.addtask([this, sockfd]()
                {
                    s2c_set_illu_result ret;
                    auto pd = _server->getdata();
                    ret.illu = pd.illu;
                    send(sockfd, (const char*)&ret, sizeof(ret), 0);
                });
            }
            break;

            default:
            {
                ERROR("send_node 收到非法包");
                return -1;
            }
        }
        
        //消息前移
        int size = client->get_recvlen() - ph->length;//未处理size
        memcpy(client->get_recvbuf(), client->get_recvbuf() + ph->length, size);
        client->set_recvlen(size);
    } 
    return 0;    
}

void 
m_group_node::send_s2c_heart(SOCKET sockfd)
{
    s2c_heart h;
    send(sockfd, (const char*)&h, sizeof(h), 0);
}

void
m_group_node::send_data_to_pc(SOCKET sockfd)
{
    c2s_get_data_result data;
    auto pd = _server->getdata();
    data.Temp = pd.temp;
    data.Rh = pd.rh;
    data.illu = pd.illu;
    data.Db = pd.db;
    send(sockfd, (const char*)&data, sizeof(data), 0);
}
