/*
 * Create time: 2023.08.16
 * Author: Lns-XueFeng
 * Function: 导入其他源文件的示例函数，以便于运行与查看
 * Environment: Windows 10, Visual Studio 2022
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock.h>

#define HELLO_CLIENT_WIN "hello_client_win"
#define TCP_CLIENT_WIN "tcp_client_win"

void error_handling(char* message);
int hello_client_win(int argc, char* argv[]);
int tcp_client_win(int argc, char* argv[]);

int main(int argc, char* argv[])
{
    if (argc == 3)
    {
        char user_input[100];
        int sr = scanf_s("%s", user_input, 100);

        // Windows上的客户端网络编程基本范式
        // Linux稍微不同，具体在进行服务器编程时查询即可

        if (strcmp(HELLO_CLIENT_WIN, user_input) == 0)
        {
            printf("CClient starting...\n");
            hello_client_win(argc, argv);  
        }
        else if (strcmp(TCP_CLIENT_WIN, user_input) == 0)
        {
            printf("CClient starting...\n");
            tcp_client_win(argc, argv);
        }
    }

    return 0;
}

/*
 * hello_client_win: 展示了基本的winsock客户端的基本范式
 * 1.Winsock的初始化
 * 2.socket套接字的建立
 * 3.connect客户端连接服务端
 * 4.recv接收服务器发送的字节数据
*/

int hello_client_win(int argc, char* argv[])
{
    WSADATA wsa_data;
    SOCKET h_socket;
    SOCKADDR_IN serv_addr;

    char message[30];
    int strlen;

    if (argc != 3)
    {
        printf("Usage: %s <ip> <port>\n", argv[0]);
        exit(1);
    }

    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
    {
        error_handling("WSAStartup fail");
    }

    h_socket = socket(PF_INET, SOCK_STREAM, 0);
    if (h_socket == INVALID_SOCKET)
    {
        error_handling("socket fail");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(h_socket, (SOCKADDR*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
    {
        error_handling("connect fail");
    }

    strlen = recv(h_socket, message, sizeof(message) - 1, 0);
    if (strlen == -1)
    {
        error_handling("recv fail");
    }
    printf("Message from server: %s \n", message);

    closesocket(h_socket);
    WSACleanup();

    return 0;
}

/*
 * tcp_client_win: 用于验证TCP套接字“传输的数据不存在数据边界”
*/

int tcp_client_win(int argc, char* argv[])
{
    WSADATA wsa_data;
    SOCKET h_socket;
    SOCKADDR_IN serv_addr;

    char message[30] ;
    int strlen = 0, read_len = 0, index = 0;

    if (argc != 3)
    {
        printf("Usage: %s <ip> <port>\n", argv[0]);
        exit(1);
    }

    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
    {
        error_handling("WSAStartup fail");
    }

    h_socket = socket(PF_INET, SOCK_STREAM, 0);
    if (h_socket == INVALID_SOCKET)
    {
        error_handling("socket fail");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(h_socket, (SOCKADDR*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
    {
        error_handling("connect fail");
    }

    while (read_len = recv(h_socket, &message[index++], 1, 0))
    {
        if (read_len == -1)
        {
            error_handling("recv fail");
        }
        printf("%d\n", read_len);
        strlen += read_len;
        // if (message[index - 1] == '\0') { break; }
    }

    printf("Message from server: %s \n", message);
    printf("Message length: %d \n", strlen);

    closesocket(h_socket);
    WSACleanup();

    return 0;
}

void error_handling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
