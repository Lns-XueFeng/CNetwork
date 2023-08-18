/*
 * Create time: 2023.08.16
 * Author: Lns-XueFeng
 * Function: 导入其他源文件的示例函数，以便于运行与查看
 * Environment: Windows 10, Visual Studio 2022
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock2.h>

#define BUF_SIZE 1024
#define HELLO_SERVER_WIN "hello_server_win"
#define ECHO_SERVER_WIN "echo_server_win"

void error_handling(char* message);
int hello_server_win(int argc, char* argv[]);
int echo_server_win(int argc, char* argv[]);

int main(int argc, char* argv[])
{
    if (argc == 2)
    {
        char user_input[100];
        int sr = scanf_s("%s", user_input, 100);

        // Windows上的客户端网络编程基本范式
        // Linux稍微不同，具体在进行服务器编程时查询即可

        if (strcmp(HELLO_SERVER_WIN, user_input) == 0)
        {
            printf("HelloServer starting...\n");
            hello_server_win(argc, argv);
        }
        else if (strcmp(ECHO_SERVER_WIN, user_input) == 0)
        {
            printf("EchoServer starting...\n");
            echo_server_win(argc, argv);
        }
    }

    return 0;
}

/*
 * hello_server_win: 展示了基本的winsock服务端的基本范式
 * 1.Winsock的初始化
 * 2.socket套接字的建立
 * 3.bind分配IP地址和端口号
 * 4.listen监听客户端连接
 * 5.accept接受客户端的连接请求
 * 6.send发送字节数据至客户端
*/

int hello_server_win(int argc, char* argv[])
{
    WSADATA wsa_data;
    SOCKET serv_sock, clnt_sock;
    SOCKADDR_IN serv_addr, clnt_addr;

    int sz_clnt_addr;
    char message[] = "Hello World!";

    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
    {
        error_handling("WSAStartup fail");
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == INVALID_SOCKET)
    {
        error_handling("socket fail");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (SOCKADDR*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
    {
        error_handling("bind fail");
    }

    if (listen(serv_sock, 5) == SOCKET_ERROR)
    {
        error_handling("listen fail");
    }

    sz_clnt_addr = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (SOCKADDR*)&clnt_addr, &sz_clnt_addr);
    if (clnt_sock == INVALID_SOCKET)
    {
        error_handling("accept fail");
    }

    send(clnt_sock, message, sizeof(message), 0);

    closesocket(clnt_sock);
    closesocket(serv_sock);
    WSACleanup();

    return 0;
}

/*
 * echo_server_win: 回声服务端
 * 将客户端传递而来的字符串原封不动的返回
 * 仅仅响应五次客户端发出的请求
*/

int echo_server_win(int argc, char* argv[])
{
    WSADATA wsa_data;
    SOCKET serv_sock, clnt_sock;
    SOCKADDR_IN serv_addr, clnt_addr;

    char message[BUF_SIZE];
    int str_len, i = 0, clnt_addr_len;

    if (argc != 2) 
    {
        printf("Usage: %s <port>", argv[0]);
        exit(1);
    }

    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
    {
        error_handling("WSAStartup fail");
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == INVALID_SOCKET) 
    {
        error_handling("socket fail");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (SOCKADDR*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
    {
        error_handling("bind fail");
    }

    if (listen(serv_sock, 5) == SOCKET_ERROR)
    {
        error_handling("listen fail");
    }

    clnt_addr_len = sizeof(clnt_addr);

    while (1)
    {
        if (i == 5) { break; }
  
        clnt_sock = accept(serv_sock, (SOCKADDR*)&clnt_addr, &clnt_addr_len);
        if (clnt_sock == INVALID_SOCKET)
        {
            error_handling("accept fail");
        }
        
        printf("Connected client %d\n", i + 1);
        while ((str_len = recv(clnt_sock, message, BUF_SIZE, 0)) != 0)
        {
            send(clnt_sock, message, str_len, 0);
        }

        closesocket(clnt_sock);
        i++;
    }

    closesocket(serv_sock);
    WSACleanup();
    return 0;

}

void error_handling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
