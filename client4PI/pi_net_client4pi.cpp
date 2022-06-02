/*************************************************************************
	> File Name: mark_client.cpp
	> Author:FW1509 
	> Remarks: Mark客户端
	> Created Time: Fri 20 May 2022 08:37:25 PM CST
 ************************************************************************/

#include<iostream>
#include"m_client.h"

int main()
{
    //SET_LEVEL(DEBUG);
    //SET_LEVEL(INFO);

    m_client c;
    c.m_init();
    c.m_connect("xxx.xxx.xxx.xxx", 8888);//xxx.xxx.xxx.xxx is your own ip addr
    c.m_start();
    c.m_work();
}


