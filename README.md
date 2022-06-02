# Environmental_Monitoring_System
Comprehensive course design of Internet of Things application system, environmental indicator monitoring system based on Raspberry Pi 4B


# Quick understanding
This project is a comprehensive course design of the Internet of Things application system of Henan University of Technology. The specific application is an environmental indicator monitoring system, which is used for the detection and management of data such as temperature, humidity, light, and flame warning in the environment. This project mainly adopts the C/S architecture, and the overall form is a single server and multiple clients. The server side is mainly used for data aggregation and forwarding, and the client side is used for data upload and data acquisition.
The client side is subdivided into the Raspberry Pi side and the PC side. The Raspberry Pi side is used to receive sensor data and upload it to the server side; the PC side uses QT to write a GUI to request to receive and display the server side data.
The server side is set up on the cloud server to receive and save the data on the Raspberry Pi side and forward it to the PC side for display. The server side adopts technologies such as multi-threading and epoll to improve overall concurrency and availability. The C/S adopts a custom application layer protocol as a whole (the transport layer protocol uses TCP) for data transmission to ensure the integrity of the overall data.
Environmental data collection in the project uses Raspberry Pi with several sensors. The main sensors/hardware used are: Raspberry Pi 4B, ssd1306 display screen, dht11 temperature and humidity sensor, and LM2904 decibel sensor. The sensor data is mainly acquired using python scripts, and summarized and managed through shell scripts.

---
本项目为河南工业大学物联网应用系统综合课设，具体应用为环境指标监控系统，用于环境内温度、湿度、光照、火焰预警等数据的检测与管理。本项目主要采用C/S架构，总体形式为单server端多client端。server端主要进行数据汇总与转发，client端用于数据上传与数据获取。
client端细分为树莓派端与PC端。树莓派端用于接收传感器数据并上传至server端；PC端使用QT编写GUI，用于请求接收server端数据并显示。
server端架设至云服务器，用于接收并保存树莓派端的数据并转发至PC端显示。server端采用多线程、epoll等技术提高整体并发与可用性。C/S之间整体采用自定义应用层协议(传输层协议使用TCP)进行数据传输，来保证整体数据的完整性。
项目中环境数据的采集使用树莓派搭配若干传感器进行，主要使用的传感器/硬件有：树莓派4B、ssd1306显示屏、dht11温湿度传感器、LM2904分贝传感器。传感器数据主要使用python脚本进行获取，并通过shell脚本进行汇总与管理。
  
  # project directory
- client4PC    client端 for PC(windows)

- client4PI     client端 for 树莓派

- hardware4PI  树莓派端传感器数据获取脚本

- public     公共源码库部分

- server     server端 for linux

# Deployment Instructions
##  1.Sensor Connection Instructions
- ssd1306显示屏接两个I2C1口，引脚号为3、5。
- dht11温湿度传感器接GPIO16口，引脚号为36。
- LM2904分贝传感器接esp8266，esp8266接树莓派ttyUSB0口。


##  2.Sensor Script Deployment Instructions
- 后台运行getData.py即可。
- 参考命令: /usr/bin/python3 /path/getData.py &
- 需要修改的内容有：

```bash
$ vim getData.py #修改87、112行的脚本位置
$ vim getData.sh #修改其中的脚本位置
$ vim getIndoorD.sh #修改数据输出位置与脚本位置
$ vim getIndoorT.sh #修改数据输出位置与脚本位置
```
## 3.Client program deployment instructions
- client4PI目录内为树莓派客户端，部署命令如下:
```bash
$ cd Pi_Net/
$ cd client4PI/
$ vim pi_net_client4pi.cpp +18 #修改服务端ip端口
$ vim m_recv_node.cpp +185 #修改光照值储存位置
$ vim m_send_node.cpp +138 #修改温湿度值与声贝值储存位置
$ make
$ ./pi_net_client4pi #启动
```
- client4PC目录内为PC(windows)客户端，需要在QT环境下编译。(编译前需修改pi_net.cpp +70处的ip端口)

## 4.Server program deployment instructions
- 首先进行MySql相关配置
```bash
$ ps -ef | grep mysqld #首先确保环境中MySql已启动
$ sudo yum install mysql-devel #其次确保已安装MySql的开发包 mysql-devel (centos) libmysqlclient-dev(apt install --ubuntu)
mysql> create database pi_net; #最后在MySql中新建pi_net库
```
- server目录内为服务端，部署命令如下：
```bash
$ cd Pi_Net/
$ cd server/
$ vim m_server.cpp +45 #修改数据库登录用户/密码
$ make
$ ./pi_net #启动
```
# Copyright statement
该项目签署了 Apache-2.0 授权许可，详情请参阅 LICENSE
