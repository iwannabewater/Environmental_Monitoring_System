#!/usr/bin/python3

import chardet
import time
import re
import serial

ser = serial.Serial(
        port='/dev/ttyUSB0',
        baudrate = 115200,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS,
        timeout=1
)

line = ser.readline()
line = line.decode('utf-8')#改编码

searchObj = re.search( r'([0-9]+)', line, re.M | re.I)

if searchObj:
   print ("dB:", searchObj.group(1))
