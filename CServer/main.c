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
#define OP_SERVER_WIN "op_server_win"
#define UECHO_SERVER_WIN "uecho_server_win"

void error_handling(char* message);
int hello_server_win(int argc, char* argv[]);
int echo_server_win(int argc, char* argv[]);
int op_server_win(int argc, char* argv[]);
int uecho_server_win(int argc, char* argv[]);

int main(int argc, char* argv[])
{
    if (argc == 2)
    {
        char user_input[100];

        fputs("Which server you want fire: ", stdout);
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
        else if (strcmp(OP_SERVER_WIN, user_input) == 0)
        {
            printf("OpServer starting...\n");
            op_server_win(argc, argv);
        }
        else if (strcmp(UECHO_SERVER_WIN, user_input) == 0)
        {
            printf("UechoServer starting...\n");
            uecho_server_win(argc, argv);
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
 * 功能：将客户端传递而来的字符串原封不动的返回，限制为五个客户端连接
 * 目的：体验编写一个较为完整的服务端
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

/*
 * op_server_win: 运算服务端
 * 功能：接收来自客户端的需计算数据并返回计算结果
 * 目的：用于体验设计应用层协议（规则）的一整个完整过程
 */

#define OPSIZE 4

int op_server_win(int argc, char* argv[])
{
    WSADATA wsa_data;
    SOCKET clnt_sock, serv_sock;
    SOCKADDR_IN clnt_addr, serv_addr;

    int recv_len;
    char message[BUF_SIZE];
    int clnt_addr_len;

    int count;
    char oper_t, add = '+', sub = '-', mul = '*';
    int result = 0;

    if (argc != 2)
    {
        printf("Usage %s <port>", argv[0]);
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

    memset(&serv_addr, 0, sizeof(serv_sock));
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
    for (int index = 0; index < 5; index++)
    {
        clnt_sock = accept(serv_sock, (SOCKADDR*)&clnt_addr, &clnt_addr_len);
        if (clnt_sock == INVALID_SOCKET)
        {
            error_handling("accept fail");
        }

        recv_len = recv(clnt_sock, message, sizeof(message), 0);
        if (recv_len == -1)
        {
            error_handling("recv fail");
        }

        count = (int)message[0];   // 总运算数
        oper_t = message[recv_len - 1];   // 运算符

        switch (oper_t)
        {
        case '+': 
            for (int index = 0; index < count; index++) result = result + message[index * OPSIZE + 1]; break;
        case '-': 
            for (int index = 0; index < count; index++) result = result - message[index * OPSIZE + 1]; break;
        case '*': 
            for (int index = 0; index < count; index++) result = result * message[index * OPSIZE + 1]; break;
        default: 
            printf("caculator fail"); break;
        }

        send(clnt_sock, (char*)&result, sizeof(result), 0);
        closesocket(clnt_sock);
    }
  
    closesocket(serv_sock);
    WSACleanup();
    return EXIT_SUCCESS;
}

int uecho_server_win(int argc, char* argv[])
{
    WSADATA wsa_data;
    SOCKET serv_sock;
    SOCKADDR_IN serv_addr, clnt_addr;
    
    char message[30];
    int str_len, clnt_adr_size;

    if (argc != 2)
    {
        printf("Usage %s <port>", argv[0]);
    }

    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
    {
        error_handling("WSAStartup fail");
    }

    serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (serv_sock == INVALID_SOCKET)
    {
        error_handling("socket fail");
    }

    memset(&serv_addr, 0, sizeof(serv_sock));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (SOCKADDR*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
    {
        error_handling("bind fail");
    }

    while (1)
    {
        clnt_adr_size = sizeof(clnt_addr);
        str_len = recvfrom(serv_sock, message, 30, 0, (SOCKADDR*)&clnt_addr, &clnt_adr_size);
        sendto(serv_sock, message, strlen, 0, (SOCKADDR*)&clnt_addr, sizeof(clnt_addr));
    }
    
    closesocket(serv_sock);
    WSACleanup();
    return EXIT_SUCCESS;
}

void error_handling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
