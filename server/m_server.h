/*************************************************************************
	> File Name: m_server.h
	> Author:FW1509 
	> Remarks: 服务端类 最外层
	> Created Time: Fri 20 May 2022 07:59:45 PM CST
 ************************************************************************/

#ifndef _M_SERVER_H_
#define _M_SERVER_H_

#include<unistd.h>
#include<sys/types.h>
#include<sys/epoll.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#include<string.h>
#include<errno.h>
#include<atomic>

#include"m_group_node.h"
#include"m_db_mgr.h"
#include"../public/log.h"

class m_server
{
public:
    using SOCKET = int;
    using task = std::function<void()>;
    static constexpr SOCKET INVALID_SOCKET = (SOCKET)(~0);
    static constexpr int SOCKET_ERROR = -1;

private:
    struct pdata
    {
        int illu;//光照
        int temp;//温度
        int rh;//湿度
        int db;//分贝
    };
    std::atomic<int> _data_illu;
    std::atomic<int> _data_temp;
    std::atomic<int> _data_rh;
    std::atomic<int> _data_db;

public:
    m_server();
    virtual ~m_server();
    m_server(const m_server&) = delete;
    m_server& operator= (const m_server&) = delete;

    /* 
    * 环境/数据初始化
    * 返回0为成功
    */
    int m_init();

    /*
    * 绑定ip/端口
    * 返回0为成功
    */
    int m_bind(const char* ip, unsigned short port);

    /*
    * 开始监听端口
    * n: 连接队列大小
    * 返回0为成功
    */
    int m_listen(int n);

    /*
    * 接受客户端连接
    * 返回0为成功
    */
    int m_accept();

    // 关闭server
    void m_close();

    // 启动子节点
    void m_start(int size);

    //开始工作
    void m_work();

    //设置data temp/rh
    void setdata_tr(int t, int r, int db)
    {
        _data_temp = t;
        _data_rh = r;
        _data_db = db;
    }

    //设置data illu
    void setdata_i(int i)
    {
        _data_illu = i;
    }

    //获取数据
    pdata getdata()
    {
        pdata ret;
        ret.illu = _data_illu;
        ret.temp = _data_temp;
        ret.rh = _data_rh;
        ret.db = _data_db;
        return ret;
    }

private:
    //添加新客户端
    void addclient_main(m_client_node* client);

    //保存数据
    void savedata();

private:
    //服务器socket
    SOCKET _sock;

    //数据库管理者
    m_db_mgr* _db;

    //子线程
    std::vector<m_group_node*> _cell_servers;

    //lock
    std::mutex _mutex_task;//任务队列锁
    std::mutex _mutex_id;//主键锁
    std::mutex _mutex_gnode;//gnode锁
};

#endif
