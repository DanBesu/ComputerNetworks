// exists on all platforms

#include <stdio.h>

// this section will only be compiled on NON Windows platforms
#ifndef WIN32
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#define closesocket close
typedef int SOCKET;

#else
// for windows
#include<WinSock2.h>
// for uint16_t an so
#include<stdint.h>
#pragma comment(lib,"Ws2_32.lib")
#endif
//#include<netinet/in.h>

int main() {
    SOCKET s;
    struct sockaddr_in server, client;
    int c, l, err;

    // init socket library on windows
#ifdef WIN32
    WSADATA wsadata;
    if(WSAStartup(MAKEWORD(2, 2), &wsadata) < 0){
        printf("Error at initialising the socket library");
        return -1;
    }
#endif
    // create socket
    s = socket(AF_INET, SOCK_STREAM, 0);
    if(s < 0){
        printf("Socket creation error");
        return 1;
    }

    // server
    memset(&server, 0, sizeof(server));
    server.sin_port = htons(1234);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;

    // bind
    if(bind(s, (struct sockaddr *) &server, sizeof(server) < 0)){
        printf("Bind error");
        return 1;
    }

    listen(s, 5);

    // client
    l = sizeof(client);
    memset(&client, 0, sizeof(client));

    while(1){
        uint16_t a, n, suma;
        printf("Wainting for conections...");
        c = accept(c, (struct sockaddr *) &client, &l);
        err = errno;

#ifdef WIN32
        err = WSAGetLastError();
#endif
        if(c < 0){
            printf("accept error %d", err);
        }
        printf("Incomming connected client from: %s:%d", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

        int res = recv(c, (char*)&n, sizeof(n), 0);
        if(res != sizeof(n))
            printf("Error for operand n\n");

        int i;
        suma = 0;
        for(i = 0; i < n; ++i){
            res = recv(c, (char*)&a, sizeof(a), 0);
            if(res != sizeof(n))
                printf("Error for operand n\n");
            a = ntohs(a);
            suma = suma + a;
            a = htons(a);
        }
        suma = htons(suma);
        res = send(c, (char*)&suma, sizeof(suma), 0);

        if(res != sizeof(n))
            printf("Error for operand n\n");

        closesocket(0);
    }
#ifdef WIN32
    WSACleanup();
#endif
}