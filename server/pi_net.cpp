/*************************************************************************
	> File Name: mark_server.cpp
	> Author:FW1509 
	> Remarks: Mark服务端
	> Created Time: Thu 27 Jan 2022 07:05:22 PM CST
 ************************************************************************/

#include<iostream>
#include"m_server.h"

int main()
{
    SET_LEVEL(DEBUG);
    //SET_LEVEL(INFO);
    //SET_LOG_PATH("./", "Mark.log", INFO);

    m_server s;
    s.m_init();
    s.m_bind(NULL, 8888);
    s.m_listen(5);
    s.m_start(4);
    s.m_work();
}
