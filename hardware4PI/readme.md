# getData.py 
&emsp;&emsp;此脚本主要用于ssd1306显示屏显示内容，显示的数据通过调用`getData.sh`获取。此脚本最好设置开机自启动，比较方便，另外显示屏的刷新速率大概为一秒一次。

&emsp;
# getData.sh
&emsp;&emsp;此脚本主要是用于汇总传感器数据，通过调用`indoor`目录里对应传感器的脚本来获取数据。

&emsp;
# indoor
&emsp;&emsp;此目录下储存传感器对应的获取数据的脚本，以及相关数据处理脚本。