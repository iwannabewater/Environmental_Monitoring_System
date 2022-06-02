#ifndef PI_NET_H
#define PI_NET_H

#include <QWidget>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QTcpSocket>
#include <QTime>
#include <QTimer>
#include <QIntValidator>
#include <QPainter>
#include <list>
#include "m_datagram.h"

class pi_net : public QWidget
{
    Q_OBJECT

public:
    pi_net(QWidget *parent = nullptr);
    ~pi_net();

//与按钮交互，故函数都设置为槽函数
private slots:
    void slot_connected(); //处理成功连接到服务器的槽
    void slot_sendmessage(); //发送消息到服务器的槽
    void slot_recvmessage(); //接收来自服务器的消息的槽
    void slot_disconnect(); //取消与服务器连接的槽
    void botton_send();//发送
    void paintEvent(QPaintEvent *);//重写函数

private:
    //tcp
    QTcpSocket* client_pc;

    //计时获取
    QTimer *timer;

    //数据存储
    std::list<int> _list_t;//温度
    std::list<int> _list_r;//湿度
    std::list<int> _list_d;//分贝

    //文本
    QLabel *text1;
    QLabel *text2;
    QLabel *text_Temp;
    QLabel *text_Rh;
    QLabel *text_illu;
    QLabel *text_db;

    QTextEdit *te1;
    QLineEdit *le1;
    QPushButton *b2;
    QPushButton *b3;
};
#endif // PI_NET_H
