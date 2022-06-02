# log
&emsp;&emsp;此类为自实现日志类。</br>
&emsp;&emsp;其中实现了六种日志等级，以及日志输出的等级设置/重定向。

&emsp;
# m_timer
&emsp;&emsp;此类为计时器类，是为了方便在此项目中统计时间而创建的。</br>
&emsp;&emsp;此类通过C++11的 `std::chrono` 类实现，总体比较简单，但是分为普通实现和线程安全实现。即通过加锁来保证线程安全，但是也有不加锁的版本，来避免锁的开销。

&emsp;
# m_clock
&emsp;&emsp;此类是为了方便获取高精度时间而创建的，在我的规划中用于心跳检测相关内容。</br>
&emsp;&emsp;此类中仅有一个静态方法来获取当前的时间戳，精度为毫秒。

&emsp;
# m_semaphore
&emsp;&emsp;此类主要是为了线程安全退出而创建的。</br>
&emsp;&emsp;类中声明了 `wait()` 和 `wakeup()` 方法。在 `wait()` 方法调用后调用线程将会阻塞，直至其他线程调用 `wakeup()` 进行唤醒。</br>
&emsp;&emsp;当子线程需要退出时，首先在主线程中调用 `wait()`，并在子线程执行内容末尾调用 `wakeup()`，即可确保子线程安全退出后主线程继续执行。

&emsp;
# m_thread
&emsp;&emsp;此类主要是为了方便创建子线程而创建的。</br>
&emsp;&emsp;在创建该类对象后，调用其 `start(event, event, event)` 方法，传入三个事件，即可完成线程的创建和启动。</br>
&emsp;&emsp;三个事件分别为创建事件、运行事件和销毁事件，运行顺序由左至右。</br>
&emsp;&emsp;事件类型实际为 `std::function<void(m_thread*)>`，所以可以传入普通函数、lambda表达式等。</br>
&emsp;&emsp;线程的退出通过 `close()`(安全退出) 和 `exit()`(不安全退出) 来进行。

&emsp;
# m_datagram
&emsp;&emsp;数据报文部分。</br>
&emsp;&emsp;一个完整的数据报文由报头 `struct header` 和其数据部分组成。

&emsp;
# m_db
&emsp;&emsp;此类主要是封装了一下C++连接操作MySql的过程，使得项目中对数据库的操作更加方便。</br>
&emsp;&emsp;使用本类的前提条件是需要确保已经安装MySql数据库系统，并且安装其开发包 `mysql-devel`。</br></br>
&emsp;&emsp;为了确保线程安全，需要在主线程的起始位置添加 `mysql_library_init(0, nullptr, nullptr)` 并在主线程的末尾位置添加 `mysql_library_end();`。</br>
* 参考地址:</br>
https://dev.mysql.com/doc/c-api/8.0/en/mysql-thread-init.html</br>
https://dev.mysql.com/doc/c-api/8.0/en/mysql-library-init.html

