#!/bin/bash

#分贝部分
/usr/bin/bash /home/pi/Pi_Net/hardware4PI/indoor/getIndoorD.sh
#光照强度
illupath="/home/pi/Pi_Net/hardware4PI/indoor/data_illu"
if [ ! -f $illupath ];then
    echo "illu: -1"
else
    cat $illupath
fi
#室内温湿度
/usr/bin/bash /home/pi/Pi_Net/hardware4PI/indoor/getIndoorT.sh
#ip获取 优先检测wlan0随后检测eth0
wlan=`ifconfig wlan0 | grep inet | awk 'NR==1{print $2}'`
eth=`ifconfig eth0 | grep inet | awk 'NR==1{print $2}'`
if [ -n "$wlan" ];then
    echo "wlan0: $wlan"
elif [ -n "$eth" ];then
    echo "eth0: $eth"
else
    echo "No internet connection"
fi
