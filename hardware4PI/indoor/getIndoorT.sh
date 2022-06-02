#!/bin/bash

ret=`/usr/bin/python3 /home/pi/Pi_Net/hardware4PI/indoor/getIndoorT.py | grep T`

if [[ -n $ret ]];then
    echo $ret > /home/pi/Pi_Net/hardware4PI/indoor/data
    echo $ret
else
    cat /home/pi/Pi_Net/hardware4PI/indoor/data
fi
