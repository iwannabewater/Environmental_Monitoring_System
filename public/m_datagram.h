/*************************************************************************
	> File Name: m_datagram.h
	> Author:FW1509 
	> Remarks: 数据报文协议
	> Created Time: Fri 20 May 2022 04:56:08 PM CST
 ************************************************************************/

#ifndef _M_DATAGRAM_H_
#define _M_DATAGRAM_H_

/*
* 命令(报文)类型
*/
enum cmd
{
    CMD_C2S_HEART,              //心跳 client to server
    CMD_S2C_HEART,              //心跳 server to client
    CMD_C2S_DATA,               //数据 树莓派->server
    CMD_C2S_DATA_RESULT,        //数据传输结果 树莓派->server
    CMD_C2S_GET_DATA,           //数据获取 PC->server
    CMD_C2S_GET_DATA_RESULT,    //数据获取 PC->server
    CMD_C2S_SET_ILLU,           //设置光照 PC->server
    CMD_S2C_SET_ILLU_RESULT,    //设置光照结果 server->PC
    CMD_C2S_GET_ILLU,           //获取光照 树莓派->server
    CMD_S2C_GET_ILLU_RESULT,    //获取光照结果 server->树莓派
};

/*
* 报头
*    cmd: 报文命令
* length: 报文长度
*/
struct header
{
    short cmd;
    short length;
};

/*
* 心跳报文 C2S
*/
struct c2s_heart : public header
{
    c2s_heart()
    {
        this->cmd = CMD_C2S_HEART;
        this->length = sizeof(c2s_heart);
    }
};

/*
* 心跳报文 S2C
*/
struct s2c_heart : public header
{
    s2c_heart()
    {
        this->cmd = CMD_S2C_HEART;
        this->length = sizeof(s2c_heart);
    }
};

/*
* 数据报文 树莓派->server
*/
struct c2s_data : public header
{
    c2s_data()
    {
        this->cmd = CMD_C2S_DATA;
        this->length = sizeof(c2s_data);
    }
    int id;//传感器id
    int Temp;//室内温度
    int Rh;//室内湿度
    int Db;//室内声贝
};

/*
* 数据传输结果 树莓派->server
*/
struct c2s_data_result : public header
{
    c2s_data_result()
    {
        this->cmd = CMD_C2S_DATA_RESULT;
        this->length = sizeof(c2s_data_result);
    }
    int result;
};

/*
* 数据获取 PC->server
*/
struct c2s_get_data : public header
{
    c2s_get_data()
    {
        this->cmd = CMD_C2S_GET_DATA;
        this->length = sizeof(c2s_get_data);
    }
};

/*
* 数据获取 PC->server
*/
struct c2s_get_data_result : public header
{
    c2s_get_data_result()
    {
        this->cmd = CMD_C2S_GET_DATA_RESULT;
        this->length = sizeof(c2s_get_data_result);
    }
    int Temp;//室内温度
    int Rh;//室内湿度
    int illu;//光照
    int Db;//室内声贝
};

/*
* 设置光照 PC->server
*/
struct c2s_set_illu : public header
{
    c2s_set_illu()
    {
        this->cmd = CMD_C2S_SET_ILLU;
        this->length = sizeof(c2s_set_illu);
    }
    int illu;
};

/*
* 设置光照结果 server->PC
*/
struct s2c_set_illu_result : public header
{
    s2c_set_illu_result()
    {
        this->cmd = CMD_S2C_SET_ILLU_RESULT;
        this->length = sizeof(s2c_set_illu_result);
    }
    int illu;
};

/*
* 获取光照 树莓派->server
*/
struct c2s_get_illu : public header
{
    c2s_get_illu()
    {
        this->cmd = CMD_C2S_GET_ILLU;
        this->length = sizeof(c2s_get_illu);
    }
};

/*
* 获取光照结果 server->树莓派
*/
struct s2c_get_illu_result : public header
{
    s2c_get_illu_result()
    {
        this->cmd = CMD_S2C_GET_ILLU_RESULT;
        this->length = sizeof(s2c_get_illu_result);
    }
    int illu;
};

#endif
