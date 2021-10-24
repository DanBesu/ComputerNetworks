#include <stdio.h>
#ifndef WIN32
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<string.h>
#include<arpa/net.h>
#include<unistd.h>
#include<errno.h>
#define  closesocket close
typedef int SOCKET;
#else
#include <WinSock2.h>
#include<stdint.h> // for uint16_t #include<cstdint>
#pragma comment(lib, "Ws2_32.lib") //
#endif


int main() {
    SOCKET sock;
    struct sockaddr_in server, client;
    int c, l, err;

#ifdef WIN32
    WSADATA wsaData;
    if(WSAStartup(MAKEWORD(2, 2), &wsaData) < 0){
        printf("Error initialising the socket library");
        return -1;
    }
#endif

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0){
        printf("Socket creation error");
        return 1;
    }

    memset(&server, 0, sizeof(server));
    server.sin_port = htons(1234);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;

    if(bind(sock, (struct sockaddr_in*)&server, sizeof(server)) < 0){
        printf("BINDING ERROR");
        return 1;
    }

    listen(sock, 5);
    l = sizeof(client);
    memset(&client, 0, sizeof(client));

    while(1){
        uint16_t len;

        printf("\nListening for connections...\n");
        c = accept(sock, (struct sockaddr*) &client, &l);
        err = errno;
#ifdef WIN32
        err = WSAGetLastError();
#endif
        if(c < 0){
            printf("Accept error: %d", err);
            continue;
        }

        printf("Incoming connected client: %s %d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

        int response = recv(c, (char *)&len, sizeof(len), 0);
        if(sizeof(len) != response){
            printf("error receiving the length\n");
        }

        len = ntohs(len);
        char str[len+1];

        response = recv(c, str, sizeof(char) * len, 0);
        str[len] = '\0';
        if(response < sizeof(char) * len){
            printf("error receiving the string");
        }

        uint16_t nr = 0;
        int i;
        for(i = 0; i < strlen(str); ++i){
            if(str[i] == ' ')
                ++nr;
        }
        nr = htons(nr);
        response = send(c, (char*)&nr, sizeof(nr), 0);
        if(response != sizeof(nr)){
            printf("error sending the result");
        }
    }
}
