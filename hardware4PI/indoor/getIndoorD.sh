#!/bin/bash

ret=`/usr/bin/python3 /home/pi/Pi_Net/hardware4PI/indoor/getIndoorD.py`

if [[ -n $ret ]];then
    echo $ret > /home/pi/Pi_Net/hardware4PI/indoor/data_db
    echo $ret
else
    cat /home/pi/Pi_Net/hardware4PI/indoor/data_db
fi
