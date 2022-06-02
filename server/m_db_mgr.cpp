/*************************************************************************
	> File Name: m_db_mgr.cpp
	> Author:FW1509 
	> Remarks: 基于m_db类进行数据库管理
	> Created Time: Tue 24 May 2022 07:15:45 PM CST
 ************************************************************************/

#include"m_db_mgr.h"
#include"../server/m_server.h"

m_db_mgr::m_db_mgr(m_server* server):
    _server(server),
    _status(false)
{

}

m_db_mgr::~m_db_mgr()
{

}

int 
m_db_mgr::init(const char* host, const char* user,
               const char* passwd)
{
    int ret;
    int num;

    //储存sql命令执行结果
    std::vector<char**>res;
    int row;
    int field;
    
    //无所属server
    if(_server == nullptr)
    {
        ERROR("m_db_mgr init error -- server is nullptr");
        return -4;
    }

    //连接数据库
    ret = _db.connect(host, user, passwd, "pi_net");
    if(ret == -1)
    {
        ERROR("m_db_mgr init error -- mysql_real_connect() error");
        return -1; 
    }
    else if(ret == -2)
    {  
        ERROR("m_db_mgr init error -- mysql_init() error");
        return -2;  
    }

    //查看数据库mark中表数量是否正常(正常为5)
    ret = _db.exec(SQL_GET_TABLE_NUM, &res, &row, &field);
    num = atoi(res[0][0]);
    DEBUG("m_db_mgr init 查表数量: %d", num);
    if(num == 0)//第一次使用 建表初始化
    {
        INFO("m_db_mgr init 建表初始化 start");
        //建表
        _db.exec(SQL_CREATE_DATA_TBL, &res, &row, &field);
    }
    if(num == 0 || num == 1)//数量正常 读表反序列化
    {
        INFO("m_db_mgr init 数据反序列化 start");
        //序列化操作
    }
    else//表数量异常
    {
        ERROR("m_db_mgr init error -- table count(%d) error", num);
        return -3;
    }

    _status = true;

    //若初始化返回1 否则返回0
    return num == 0 ? 1 : 0;
}

int 
m_db_mgr::insert_data(int i, int t, int r, int d)
{
    char sql[512] = { 0 };
    
    snprintf(sql, 512, SQL_INSERT_DATA, t, r, d, i);
    _db.exec(sql, nullptr, nullptr, nullptr);
    return 0;
}
