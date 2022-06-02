/*************************************************************************
	> File Name: m_db_mgr.h
	> Author:FW1509 
	> Remarks: 基于m_db类进行数据库管理
	> Created Time: Tue 24 May 2022 07:15:45 PM CST
 ************************************************************************/

#ifndef _M_DB_MGR_H
#define _M_DB_MGR_H

#include<string>
#include<unordered_map>
#include"../public/log.h"
#include"../public/m_db.h"

class m_server;

class m_db_mgr
{
public:
    m_db_mgr(m_server* server);
    virtual ~m_db_mgr();
    m_db_mgr() = delete;
    m_db_mgr(const m_db_mgr&) = delete;
    m_db_mgr& operator= (const m_db_mgr&) = delete;

    /*
    * 反序列化数据
    *
    *    host: host
    *    user: 登录用户名
    *  passwd: 登录密码
    * 
    * 返回值:
    * -4 : 失败 - 所属server不存在
    * -3 : 失败 - mark库内表数量异常
    * -2 : 失败 - 连接失败 - mysql_init() error
    * -1 : 失败 - 连接失败 - mysql_real_connect() error
    *  0 : 成功 - 读取成功
    *  1 : 成功 - 第一次使用, 初始化数据库
    */
    int init(const char* host, const char* user,
             const char* passwd);

    /*
    * 数据持久化
    * 光照 - 温度 - 湿度 - 分贝
    * 
    * 返回值:
    * -1 : 失败 - 插入失败
    *  0 : 成功 - 插入成功
    */
    int insert_data(int i, int t, int r, int d);

private:
    //所属server
    m_server* _server;

    //数据库连接
    m_db _db;
    bool _status;

private:

private:
    //查询mark库中有几张表
    static constexpr char SQL_GET_TABLE_NUM[] = R"(
    SELECT
        COUNT(TABLE_NAME)
    FROM
        information_schema.TABLES
    WHERE TABLE_SCHEMA = 'pi_net';)";

    //建表语句
    static constexpr char SQL_CREATE_DATA_TBL[] = R"(
    CREATE TABLE `pi_net_info` (
    `time` DATETIME NOT NULL,
    `temperature` mediumint unsigned NOT NULL DEFAULT '0',
    `humidity` mediumint unsigned NOT NULL DEFAULT '0',
    `decibel` mediumint unsigned NOT NULL DEFAULT '0',
    `illumination` mediumint unsigned NOT NULL DEFAULT '0',
    PRIMARY KEY (`time`));)";

    //插入数据
    static constexpr char SQL_INSERT_DATA[] = R"(
    INSERT INTO 
        pi_net_info 
    VALUES
        (now(), %d, %d, %d, %d);)";//温度、湿度、分贝、光照
};

#endif
