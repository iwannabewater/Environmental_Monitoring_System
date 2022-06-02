#include "pi_net.h"

pi_net::pi_net(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(400,400);
    setMaximumSize(400,400);
    setWindowTitle(tr("Pi_Net for PC"));

    auto mainLayout1 = new QVBoxLayout; //主框架 竖
    auto mainLayoutdraw = new QHBoxLayout;
    auto mainLayout2 = new QHBoxLayout; //子框架上 横
    auto mainLayout3 = new QHBoxLayout; //子框架下 横
    auto mainLayout4 = new QVBoxLayout; //子子框架右 竖
    auto mainLayout5 = new QVBoxLayout; //子子框架左 竖

    auto zwf = new QLabel();//占位符
    zwf->setFixedSize(400, 200);
    text1 = new QLabel(tr("状态：连接中..."));//子框架上 文本显示
    text2 = new QLabel(tr("请输入光照值"));//子框架 右边下 文本

    //数据文本
    text_illu = new QLabel(tr("光照: -"));//子框架 左下3 文本
    QFont font;
    font.setFamily("微软雅黑");
    font.setPointSize(14);//设置字号
    text_Temp = new QLabel(tr("温度: --°C"));//子框架 左下1 文本
    text_Temp->setFont(font);
    text_Rh = new QLabel(tr("湿度: --%"));//子框架 左下2 文本
    text_Rh->setFont(font);
    text_db = new QLabel(tr("分贝: --dB"));//子框架 左下3 文本
    text_db->setFont(font);

    le1 = new QLineEdit;//子框架下 右边 文本接收
    b2 = new QPushButton(tr("设置"));//子框架下 右边 确定按钮
    b3 = new QPushButton(tr("退出"));//子框架下 右边 退出按钮

    mainLayout1->addLayout(mainLayoutdraw);
    mainLayoutdraw->addWidget(zwf);
    mainLayout1->addLayout(mainLayout2);
    mainLayout1->addLayout(mainLayout3);
    mainLayout1->setStretchFactor(mainLayoutdraw, 2);
    mainLayout1->setStretchFactor(mainLayout2, 1);
    mainLayout1->setStretchFactor(mainLayout3, 1);
    mainLayout2->addWidget(text1);
    mainLayout2->addWidget(text_illu);
    //左边的数据
    mainLayout3->addLayout(mainLayout5);
    mainLayout5->addWidget(text_Temp);
    mainLayout5->addWidget(text_Rh);
    mainLayout5->addWidget(text_db);
    //右边的按钮
    mainLayout3->addLayout(mainLayout4);
    mainLayout4->addWidget(text2);
    mainLayout4->addWidget(le1);
    mainLayout4->addWidget(b2);
    mainLayout4->addWidget(b3);
    //比例设置 1:1
    mainLayout3->setStretchFactor(mainLayout4, 1);
    mainLayout3->setStretchFactor(mainLayout5, 1);
    setLayout(mainLayout1);
    setLayout(mainLayout2);
    setLayout(mainLayout3);
    setLayout(mainLayout4);

    //le1->setPlaceholderText("默认值为 1 ");
    //le1->setText("1");
    le1->setValidator(new QIntValidator(le1));
    connect(le1, SIGNAL(activated(int)), this, SLOT(accessChanged(int)));

    //绑定按钮
    connect(b2, &QAbstractButton::clicked, this, &pi_net::botton_send);
    connect(b3, &QAbstractButton::clicked, this, &pi_net::close);

    //socket那一套 在显示窗口以后进行
    client_pc = new QTcpSocket();
    client_pc->connectToHost("xxx.xxx.xxx.xxx", 8888);//xxx.xxx.xxx.xxx is your ip address

    //绑定连接信号
    //成功连接服务器的connected()信号连接到slot_connected() (注意：不是connect()信号)
    connect(client_pc,SIGNAL(connected()),this,SLOT(slot_connected()));
    //有新数据到达时的readyread()信号连接到slot_recvmessage()
    connect(client_pc,SIGNAL(readyRead()),this,SLOT(slot_recvmessage()));
    //与服务器断开连接的disconnected()信号连接到slot_disconnect()
    connect(client_pc,SIGNAL(disconnected()),this,SLOT(slot_disconnect()));

    //设置定时间隔并connect
    timer = new QTimer();
    timer->setInterval(5000);//5秒刷新一次
    connect(timer, SIGNAL(timeout()), this, SLOT(slot_sendmessage()));
    timer->start();//开始计时
    //slot_recvmessage();
}

pi_net::~pi_net()
{
}

void pi_net::slot_connected()
{
    text1->setText("状态：连接成功");
}

void pi_net::slot_sendmessage()
{
    timer->stop();
    c2s_get_data cd;
    client_pc->write((const char*)&cd, sizeof(cd));
    timer->start();
}

void pi_net::slot_recvmessage()
{
    static long long times = 0;
    QByteArray byteArray = client_pc->readAll();
    header* ph = (header*)(byteArray.data());
    switch(ph->cmd)
    {
        case CMD_C2S_GET_DATA_RESULT:
        {
            c2s_get_data_result* cgdr = (c2s_get_data_result*)ph;
            text_Temp->setText("温度: " + QString::number(cgdr->Temp) + "°C");
            text_Rh->setText("湿度: " + QString::number(cgdr->Rh) + "%");
            text_illu->setText("光照: " + QString::number(cgdr->illu));
            text_db->setText("分贝: " + QString::number(cgdr->Db) +"dB");
            //更新数据
            if(++times & 1)//刷新两次更新一次图像
            {
                _list_t.push_back(cgdr->Temp);
                if(_list_t.size() > 12)
                    _list_t.pop_front();
                _list_r.push_back(cgdr->Rh);
                if(_list_r.size() > 12)
                    _list_r.pop_front();
                _list_d.push_back(cgdr->Db);
                if(_list_d.size() > 12)
                    _list_d.pop_front();
                update();
            }
        }
        break;

        case CMD_S2C_SET_ILLU_RESULT:
        {
            s2c_set_illu_result* csir = (s2c_set_illu_result*)ph;
            text_illu->setText("光照: " + QString::number(csir->illu));
        }
        break;

        default:
        {

        }
    }
}

void pi_net::slot_disconnect()
{
    text1->setText("状态：连接中断");
    client_pc->close();
    client_pc->deleteLater();
}

void pi_net::botton_send()
{
    QString qstr = (le1->text());
    int newillu = qstr.toInt();

    c2s_set_illu csi;
    csi.illu = newillu;
    client_pc->write((const char*)&csi, sizeof(csi));
}

void pi_net::paintEvent(QPaintEvent *)
{
    //坐标轴/////////////////////////////////////////////////////////////////////////////
    QPainter painter(this);//建立QPainter并绑定widget
    QPen pen;//生成画笔
    QFont font1("微软雅黑", 10);//生成字体
    pen.setColor(Qt::gray);//笔 灰色
    pen.setStyle(Qt::SolidLine);
    pen.setWidthF(2);//此函数可定义宽度可精确到小数
    painter.setPen(pen);//画笔画布绑定
    painter.setFont(font1);//字体画布绑定
    painter.setViewport(0, 0, 400, 200);//设置画布视窗大小大小为800*800大小
    painter.setWindow(0, 0, 400, 200);//设置逻辑坐标范围
    painter.fillRect(0, 0, 400, 200, Qt::white);//背景颜色
    painter.setRenderHint(QPainter::Antialiasing,true);//抗锯齿
    painter.drawLine(15,175,375,175);//x轴
    painter.drawLine(15,15,15,175);//y轴
    painter.drawText(QPoint(369,181),QString(">t"));//x轴箭头
    painter.drawText(QPoint(7,20),QString("∧°C/%/dB"));//y轴箭头

    //绘图部分
    double lastx = 0;
    double lasty = -1.0;
    int lastv = 0;//last value
    //温度 y:20~40
    pen.setColor(Qt::red);
    painter.setPen(pen);
    for(int t : _list_t)
    {
        double t_x = lastx + 30;
        double t_y = 150 - (t - 20) * 7.5;
        if(lasty < 0)
            painter.drawPoint(QPointF(t_x, t_y));
        else
            painter.drawLine(QPointF(lastx, lasty), QPointF(t_x, t_y));
        lastx = t_x;
        lasty = t_y;
        lastv = t;
    }
    painter.drawText(QPoint(30,194),QString("—— 温度"));
    painter.drawText(QPoint(373, static_cast<int>(lasty)), QString::number(lastv));
    //湿度 y:40~100
    lastx = 0;
    lasty = -1.0;
    pen.setColor(Qt::blue);
    painter.setPen(pen);
    for(int r : _list_r)
    {
        double t_x = lastx + 30;
        double t_y = 150 - (r - 40) * 2.5;
        if(lasty < 0)
            painter.drawPoint(QPointF(t_x, t_y));
        else
            painter.drawLine(QPointF(lastx, lasty), QPointF(t_x, t_y));
        lastx = t_x;
        lasty = t_y;
        lastv = r;
    }
    painter.drawText(QPoint(150,194),QString("—— 湿度"));
    painter.drawText(QPoint(373, static_cast<int>(lasty)), QString::number(lastv));
    //分贝 y:30~50
    lastx = 0;
    lasty = -1.0;
    pen.setColor(Qt::green);
    painter.setPen(pen);
    for(int d : _list_d)
    {
        double t_x = lastx + 30;
        double t_y = 150 - (d - 30) * 7.5;
        if(lasty < 0)
            painter.drawPoint(QPointF(t_x, t_y));
        else
            painter.drawLine(QPointF(lastx, lasty), QPointF(t_x, t_y));
        lastx = t_x;
        lasty = t_y;
        lastv = d;
    }
    painter.drawText(QPoint(270,194),QString("—— 分贝"));
    painter.drawText(QPoint(373, static_cast<int>(lasty)), QString::number(lastv));
}





