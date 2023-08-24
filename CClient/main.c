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
#define HELLO_CLIENT_WIN "hello_client_win"
#define TCP_CLIENT_WIN "tcp_client_win"
#define ECHO_CLIENT_WIN "echo_client_win"
#define OP_CLIENT_WIN "op_client_win"
#define UECHO_CLIENT_WIN "uecho_client_win"

void error_handling(char* message);
int hello_client_win(int argc, char* argv[]);
int tcp_client_win(int argc, char* argv[]);
int echo_client_win(int argc, char* argv[]);
int op_client_win(int argc, char* argv[]);
int uecho_client_win(int argc, char* argv[]);

int main(int argc, char* argv[])
{
    if (argc == 3)
    {
        char user_input[100];

        fputs("Which client you want fire: ", stdout);
        int sr = scanf_s("%s", user_input, 100);

        // Windows上的客户端网络编程基本范式
        // Linux稍微不同，具体在进行服务器编程时查询即可

        if (strcmp(HELLO_CLIENT_WIN, user_input) == 0)
        {
            printf("HelloClient starting...\n");
            hello_client_win(argc, argv);  
        }
        else if (strcmp(TCP_CLIENT_WIN, user_input) == 0)
        {
            printf("TcpClient starting...\n");
            tcp_client_win(argc, argv);
        }
        else if (strcmp(ECHO_CLIENT_WIN, user_input) == 0)
        {
            printf("EchoClient starting...\n");
            echo_client_win(argc, argv);
        }
        else if (strcmp(OP_CLIENT_WIN, user_input) == 0)
        {
            printf("OpClient starting...\n");
            op_client_win(argc, argv);
        }
        else if (strcmp(UECHO_CLIENT_WIN, user_input) == 0)
        {
            printf("UechoClient starting...\n");
            uecho_client_win(argc, argv);
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
 * tcp_client_win
 * 功能：连接服务器后，接收来自服务器的数据
 * 目的：用于验证TCP套接字“传输的数据不存在数据边界”
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
        strlen += read_len;
        // if (message[index - 1] == '\0') { break; }
    }

    printf("Message from server: %s \n", message);
    printf("Message length: %d \n", strlen);

    closesocket(h_socket);
    WSACleanup();

    return 0;
}

/*
 * echo_client_win: 回声客户端
 * 功能：用于向服务器发送字符串并接收服务器原封不动的返回
 * 目的：体验编写一个较为完整的客户端
 */

int echo_client_win(int argc, char* argv[])
{
    WSADATA wsa_data;
    SOCKET clnt_sock;
    SOCKADDR_IN serv_addr;

    int str_len, mess_len, recv_len;
    char message[BUF_SIZE];

    if (argc != 3)
    {
        printf("Usage: %s <ip> <port>\n", argv[0]);
        exit(1);
    }

    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
    {
        error_handling("WSAStartup fail");
    }

    clnt_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (clnt_sock == INVALID_SOCKET)
    {
        error_handling("socket fail");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(clnt_sock, (SOCKADDR*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
    {
        error_handling("connect fail");
    }

    printf("Conneted...\n");
    while (1)
    {
        fputs("Input message(q to quit): ", stdout);
        fgets(message, BUF_SIZE, stdin);

        if (strcmp(message, "q") == 0) { break; }

        /*
          回声客户端可提前预知所发送的信息长度
          但一般情况下均无法预知，因此对于实际网络应用需要定义应用层的协议
        */
        recv_len = 0;
        mess_len = send(clnt_sock, message, strlen(message), 0);
        while (recv_len < mess_len)
        {
            str_len = recv(clnt_sock, message, BUF_SIZE - 1, 0);
            if (str_len == -1)
            {
                error_handling("recv fail");
            }
            recv_len += str_len;
        }
        message[recv_len] = 0;
        printf("Message from server: %s", message);
    }

    closesocket(clnt_sock);
    WSACleanup();
    return 0;
}

/*
 * op_client_win: 运算客户端
 * 功能：向服务器发送数字并要求进行运算以及返回
 * 目的：用于体验设计应用层协议（规则）的一整个完整过程
 */

/* 应用层协议
 * 客户端连接到服务器端后以1字节的正式形式传递待运算数字个数
 * 客户端向服务器端传送的每个整数型数据占用4字节
 * 传递整数型数据后接着传递运算符，运算符数据占用1字节
 * 选择字符+、-、*之一传送
 * 服务器端以四字节整数型向客户端传送运算结果
 * 客户端得到结果之后终止与服务器端的连接
 */

#define OPSIZE 4

int op_client_win(int argc, char* argv[])
{
    WSADATA wsa_data;
    SOCKET clnt_sock;
    struct sockaddr_in serv_addr;

    char buffer[BUF_SIZE];
    int count = 0;
    char operator;
    int send_len, recv_len = 0, new_recv_len;

    int opt_result;

    if (argc != 3)
    {
        printf("Usage: %s <ip> <port>", argv[0]);
    }

    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
    {
        error_handling("WSAStartup fail");
    }

    clnt_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (clnt_sock == INVALID_SOCKET)
    {
        error_handling("socket fail");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(clnt_sock, (SOCKADDR*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
    {
        error_handling("connect fail");
    }

    fputs("Operand count: ", stdout);
    scanf("%d", &count);
    buffer[0] = (char) count;   // 第一个是总运算个数
    for (int index = 0; index < count; index++)
    {
        printf("Operand %d: ", index + 1);
        scanf("%d", (int*)&buffer[index * OPSIZE + 1]);   // 中间是运算数
    }

    fgetc(stdin);
    fputs("Operator: ", stdout);
    scanf("%c", &buffer[count * OPSIZE + 1]);   // 最后一个是运算符

    if (send(clnt_sock, &buffer, count * OPSIZE + 2, 0) == SOCKET_ERROR)
    {
        error_handling("send fail");
    }

    new_recv_len = recv(clnt_sock, &opt_result, OPSIZE, 0);
    if (new_recv_len == -1)
    {
        error_handling("recv fail");
    }

    printf("Operator result: %d\n", opt_result);
    closesocket(clnt_sock);
    WSACleanup();
    return EXIT_SUCCESS;
}

int uecho_client_win(int argc, char* argv[])
{
    WSADATA wsa_data;
    SOCKET clnt_sock;
    struct sockaddr_in serv_addr;

    int str_len;
    int serv_addr_len;
    char message[30];

    if (argc != 3)
    {
        printf("Usage: %s <ip> <port>", argv[0]);
    }

    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
    {
        error_handling("WSAStartup fail");
    }

    clnt_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (clnt_sock == INVALID_SOCKET)
    {
        error_handling("socket fail");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));
    connect(clnt_sock, (SOCKADDR*)&serv_addr, sizeof(serv_addr));
    /*
     * 当使用了connect进行连接时，则可按照之前那样使用send、recv来接发数据
     * 当不使用connect进行连接时，专供给UDP的sendto会自动分配ip、port进行连接，recvfrom是接收数据
     */

    while (1)
    {
        fputs("Insert message(q to quit): ", stdout);
        scanf("%s", message);
        if (strcmp(message, "q") == 0) { break; }

        send(clnt_sock, message, strlen(message), 0);
        // sendto(clnt_sock, message, strlen(message), 0, (SOCKADDR*)&serv_addr, sizeof(serv_addr));
        str_len = recv(clnt_sock, message, sizeof(message) - 1, 0);
        // serv_addr_len = sizeof(serv_addr);
        // str_len = recvfrom(clnt_sock, message, strlen(message) - 1, 0, (SOCKADDR*)&serv_addr, &serv_addr_len);
        message[str_len] = 0;
        printf("Mesaage from server: %s", message);
    }
    closesocket(clnt_sock);
    WSACleanup();
    return EXIT_SUCCESS;
}

void error_handling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
