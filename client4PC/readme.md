# datagram
&emsp;&emsp;数据报文部分。</br>
&emsp;&emsp;一个完整的数据报文由报头 `struct header` 和其数据部分组成。

# pi_net
&emsp;&emsp;PC端程序的主要实现部分。采用QT实现GUI，并通过QT封装好的`QTcpSocket`部分实现网络传输。<br>
&emsp;&emsp;主要采用QT的信号绑定机制来绑定接收报文回调函数，来实现对数据报文的响应与更新。并采用QT的计时器`QTimer`以及对应信号槽定时发送报文获取数据，来保证整体业务的正常进行。
