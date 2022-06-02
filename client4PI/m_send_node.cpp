/*************************************************************************
	> File Name: m_send_node.cpp
	> Author:FW1509 
	> Remarks: 客户端send节点 向服务端send
	> Created Time: Fri 20 May 2022 08:37:25 PM CST
 ************************************************************************/

#include"m_send_node.h"

m_send_node::m_send_node(m_client* client):
    _client(client)
{

}

m_send_node::~m_send_node()
{
    DEBUG("send_node ~ start");
    close_node();
    DEBUG("send_node ~ start");
}

void
m_send_node::start()
{
    DEBUG("send_node thread start");
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
}

void
m_send_node::close_node()
{
    DEBUG("send_node thread close");
    _thread.close();
}

void 
m_send_node::func_create(m_thread* thread)
{
    DEBUG("send_node func_create() start"); 
}

void 
m_send_node::func_run(m_thread* thread)
{
    DEBUG("send_node func_run() start");
    
    //计时器->发心跳包
    m_timer timer;
    timer.update();
    double mytimer = 0.0;//心跳包计时器
    double datatimer = 0.0;//数据计时器
    
    while(_thread.is_run())
    {
        double temptimer = timer.get_sec();
        timer.update();

        //心跳包 两秒一发
        mytimer += temptimer;
        if(mytimer >= 2.0)
        {
            c2s_heart h;
            send(_sockfd, (const char*)&h, sizeof(h), 0);
            mytimer = 0.0;
        }

        //数据包/光照请求 10秒一发
        datatimer += temptimer;
        if(datatimer >= 10.0)
        {
            send_data_to_server();
            get_illu_from_server();//get_illu
            datatimer = 0.0;
        }
        
        //将缓冲区内数据加入 
        if(!_tasksbuf.empty()) 
        {
            std::lock_guard<std::mutex> lock(_mutex);
            for(auto& t : _tasksbuf)
            {
                _tasks.push_back(t);
            }
            _tasksbuf.clear();
        }
        
        //如果无任务
        if(_tasks.empty())
        {
            usleep(1000);//1ms
            continue;
        }
        
        //处理任务
        for(auto& t : _tasks)
        {
            t();
        }
        _tasks.clear();
    }
}

void 
m_send_node::func_destory(m_thread* thread)
{
    DEBUG("send_node func_destory() start");
}

void 
m_send_node::addtask(task t)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _tasksbuf.push_back(t);
}

void
m_send_node::send_data_to_server()
{
    //温湿度数据部分
    std::ifstream ifs;
    ifs.open("/home/pi/Pi_Net/hardware4PI/indoor/data", std::ios::in);
    if (!ifs.is_open())
        ERROR("read fail.");
    std::string buf_t;
    getline(ifs, buf_t);
    ifs.close();
    std::cmatch m_t;
    std::regex_match(buf_t.c_str(), m_t, std::regex(".*: ([0-9]+) *°C / ([0-9]+).*%"));

    //声贝部分
    std::ifstream ifs_d;
    ifs_d.open("/home/pi/Pi_Net/hardware4PI/indoor/data_db", std::ios::in);
    if (!ifs_d.is_open())
        ERROR("read fail.");
    std::string buf_d;
    getline(ifs_d, buf_d);
    ifs_d.close();
    std::cmatch m_d;
    std::regex_match(buf_d.c_str(), m_d, std::regex("dB: ([0-9]+).*$"));

    c2s_data data;
    data.id = 1;
    data.Temp = stoi(m_t.str(1));
    data.Rh = stoi(m_t.str(2));
    data.Db = stoi(m_d.str(1));
    send(_sockfd, (const char*)&data, sizeof(data), 0);
}

void 
m_send_node::get_illu_from_server()
{
    c2s_get_illu data;
    send(_sockfd, (const char*)&data, sizeof(data), 0);
}



