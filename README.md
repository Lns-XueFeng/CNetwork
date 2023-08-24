# Network
此项目内的代码均是适用于Windows，但Linux亦非常重要，因此在此处补充Linux与Windows的差异之处！
- CServer：集合了多个Server端函数供调用
- CClient：集合了多个Client端函数供调用
- 注意：克隆本项目后使用VS打开CNetwork.sln，相关命令参数均已配置，直接点击启动即可

## Linux
在Linux下的网络套接字编程需要用到**<sys/socket.h>**，直接导入即可！

### 服务端

```c
// 建立套接字
#include <sys/socket.h>
int socket(int domain, int type, int protocol);
/*
 * domain   套接字中使用的协议族信息
 * type     套接字数据传输类型信息
 * protocol 计算机间通信中使用的协议信息
 * 成功时返回文件描述符，失败时返回-1
 */
```

```c
// 给套接字分配地址
#include <sys/socket.h>
int bind(int sockfd, struct sockaddr* myaddr, socklen_t addrlen);
/*
 * 成功时返回文件描述符，失败时返回-1
 */
```

```c
// 监听连接请求
#include <sys/socket.h>
int listen(int sockfd, int backlog);
/*
 * 成功时返回文件描述符，失败时返回-1
 */
```

```c
// 接收连接请求
#include <sys/socket.h>
int accept(int sockfd, struct sockaddr* addr, socklen_t* addrlen);
/*
 * 成功时返回文件描述符，失败时返回-1
 */
```

### 客户端

```c
// 建立套接字
#include <sys/socket.h>
int socket(int domain, int type, int protocol);
/*
 * domain   套接字中使用的协议族信息
 * type     套接字数据传输类型信息
 * protocol 计算机间通信中使用的协议信息
 * 成功时返回文件描述符，失败时返回-1
 */
```

```c
// 连接服务端
#include <sys/socket.h>
int connect(int sockfd, struct sockaddr* serv_addr, socklen_t addrlen);
/*
 * 成功时返回文件描述符，失败时返回-1
 */
```

### 补充

Linux之上一切皆文件，因此socket其实也是Linux上的一种文件，那么文件相关的操作函数便是通用的，比如open、close、write、read等。

此外在Linux上是有已经预定了一些特殊的文件描述符的，stdin、stdout、stderr，分别是0、1、2，而其他文件创建之后则是系统会分配文件描述符。

因此就最终我们在使用Linux和Windows的API时会发现明显的不一致，但是基本套路是差不多的！

```c
// 关闭套接字
#include <unistd.h>
int close(int fd);
/*
 * fd 需要关闭的文件或套接字的文件描述符
 * 成功时返回文件描述符，失败时返回-1
 */
```

```c
// 传输数据
#include <unistd.h>
ssize_t write(int fd, const void* buf, size_t nbytes);
/*
 * fd     显示数据传输对象的文件描述符
 * buf    保存要传输数据的缓冲地址值
 * nbytes 要传输数据的字节数
 * 成功时返回写入的字节数，失败时返回-1
 */
```

```c
// 接收数据
#include <unistd.h>
ssize_t read(int fd, void* buf, size_t nbytes);
/* 
 * fd     显示数据接收对象的文件描述符
 * buf    要保存接收数据的缓冲地址值
 * nbytes 要接收数据的最大字节数
 * 成功时返回接收的字节(但遇到文件结尾则返回0)，失败时返回-1
 */
```



## Windows
在Windows下的网络套接字编程需要用到**<winsock2.h>**，需要提前在**Vissual Studio**中链接**ws2_32.lib**库！

### 服务端

```c
// Winsock的初始化
#include <winsock2.h>
int WSAStartup(WORD wVersionRequested, LPWSADATA lpWSAData);
/*
 * wVersionRequested 程序员要用的Winsock版本信息
 * lpWSAData         WSADATA结构体变量的地址值
 * 成功时返回0，失败时返回SOCKET_ERROR
 */
```

```c
// 建立套接字
#include <winsock2.h>
SOCKET socket(int af, int type, int protocol);
/*
 * af       套接字中使用的协议族信息
 * type     套接字数据传输类型信息
 * protocol 计算机间通信中使用的协议信息
 * 成功时返回套接字句柄，失败时返回INVALID_SOCKET
 */
```

```c
// 给套接字分配地址
#include <winsock2.h>
int bind(SOCKET s, const struct sockaddr* name, int namelen);
/*
 * 成功时返回0，失败时返回SOCKET_ERROR
 */
```

```c
// 监听连接请求
#include <winsock2.h>
int listen(SOCKET s, int backlog);
/*
 * 成功时返回0，失败时返回SOCKET_ERROR
 */
```

```c
// 接收连接请求
#include <winsock2.h>
SOCKET accept(SOCKET s, struct sockaddr* addr, int* addrlen);
/*
 * 成功时返回套接字句柄，失败时返回INVALID_SOCKET
 */
```

```c
// 传输数据
#include <winsock2.h>
int send(SOCKET s, const char* buf, int len, int flags);
/*
 * s     表示数据传输对象连接的套接字句柄值
 * buf   保存待传输数据的缓冲地址值
 * len   要传输的字节数
 * flags 传输数据时用到的多种选项信息
 * 成功时返回传输字节数，失败时返回SOCKET_ERROR
 */
```

```c
// 接收数据
#include <winsock2.h>
int recv(SOCKET s, const char* buf, int len, int flags);
/*
 * s     表示数据接收对象连接的套接字句柄值
 * buf   保存接收数据的缓冲地址值
 * len   能够接收的最大字节数
 * flags 接收数据时用到的多种选项信息
 * 成功时返回接收的字节(收到EOF时为0)，失败时返回SOCKET_ERROR
 */
```

```c
// 关闭套接字
#include <winsock2.h>
int closesocket(SOCKET s);
/*
 * 成功时返回0，失败时返回SOCKET_ERROR
 */
```

```c
// 关闭Winsock
#include <winsock2.h>
int WSACleaup(void);
/*
 * 成功时返回0，失败时返回SOCKET_ERROR
 */
```

### 客户端

```c
// Winsock的初始化
#include <winsock2.h>
int WSAStartup(WORD wVersionRequested, LPWSADATA lpWSAData);
/*
 * wVersionRequested 程序员要用的Winsock版本信息
 * lpWSAData         WSADATA结构体变量的地址值
 * 成功时返回0，失败时返回SOCKET_ERROR
 */
```

```c
// 建立套接字
#include <winsock2.h>
SOCKET socket(int af, int type, int protocol);
/*
 * af       套接字中使用的协议族信息
 * type     套接字数据传输类型信息
 * protocol 计算机间通信中使用的协议信息
 * 成功时返回套接字句柄，失败时返回INVALID_SOCKET
 */
```

```c
// 连接服务端
#include <winsock2.h>
int connect(int sockfd, struct sockaddr* serv_addr, socklen_t addrlen);
/*
 * 成功时返回0，失败时返回SOCKET_ERROR
 */
```

```c
// 传输数据
#include <winsock2.h>
int send(SOCKET s, const char* buf, int len, int flags);
/*
 * s     表示数据传输对象连接的套接字句柄值
 * buf   保存待传输数据的缓冲地址值
 * len   要传输的字节数
 * flags 传输数据时用到的多种选项信息
 * 成功时返回传输字节数，失败时返回SOCKET_ERROR
 */
```

```c
// 接收数据
#include <winsock2.h>
int recv(SOCKET s, const char* buf, int len, int flags);
/*
 * s     表示数据接收对象连接的套接字句柄值
 * buf   保存接收数据的缓冲地址值
 * len   能够接收的最大字节数
 * flags 接收数据时用到的多种选项信息
 * 成功时返回接收的字节(收到EOF时为0)，失败时返回SOCKET_ERROR
 */
```

```c
// 关闭套接字
#include <winsock2.h>
int closesocket(SOCKET s);
/*
 * 成功时返回0，失败时返回SOCKET_ERROR
 */
```

```c
// 关闭Winsock
#include <winsock2.h>
int WSACleaup(void);
/*
 * 成功时返回0，失败时返回SOCKET_ERROR
 */
```

### 补充

值得注意一点的是：send、recv并非Windows独有的，在Linux之中也有它们，但是一般而言在Linux环境时使用write、read即可

```c
// 补充ip地址的声明、设置、转换类型
#include <winsock2.h>

SOCKADDR_IN serv_addr;

memset(&serv_addr, 0, sizeof(serv_addr));
serv_addr.sin_family = AF_INET;
serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
serv_addr.sin_port = htons(atoi(argv[2]));

(SOCKADDR*) serv_addr;
```

```c
// UDP程序之中核心的两个函数
ssize_t sendto(int sock, void* buff, size_t nbytes, int flags, struct sockaddr* to, socklen_t addrlen);
/*
 * sock     表示数据发送对象连接的套接字
 * buff     保存待发送数据的缓冲地址值
 * nbytes   待传输的数据长度，以字节为单位
 * flags    发送数据时用到的多种选项信息
 * to       存有目标地址信息的sockaddr结构体变量的地址值
 * addrlen  传递给参数to的地址值结构体变量长度
 */
ssize_t recvfrom(int sock, void* buff, size_t nbytes, int flags, struct sockaddr* from, socklen_t addrlen);
/*
 * sock     表示数据接收对象连接的套接字
 * buff     保存接收数据的缓冲地址值
 * nbytes   可接收的最大字节数，故无法超过参数buff所指的缓冲大小
 * flags    发送数据时用到的多种选项信息
 * from     存有发送端地址信息的sockaddr结构体变量的地址值
 * addrlen  保存参数from的结构体变量长度的变量地址值
 */
```





