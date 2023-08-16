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

#define HELLO_SERVER_WIN "hello_server_win"

void error_handling(char* message);
int hello_server_win(int argc, char* argv[]);

int main(int argc, char* argv[])
{
    if (argc == 2)
    {
        char user_input[100];
        int sr = scanf_s("%s", user_input, 100);
        if (strcmp(HELLO_SERVER_WIN, user_input) == 0)
        {
            printf("CServer starting...\n");
            hello_server_win(argc, argv);
        }
    }

    return 0;
}

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

void error_handling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
