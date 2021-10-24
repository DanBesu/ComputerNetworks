#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(){

    int c;
    struct sockaddr_in server;

    int32_t a, n, suma;

    c = socket(AF_INET, SOCK_STREAM, 0);
    if(c < 0){
        printf("socket error");
        return 1;
    }
    
    memset(&server, 0, sizeof(server));
    server.sin_port = htons(1234);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("192.168.100.9");

    if(connect(c, (struct sockaddr *) &server, sizeof(server)) < 0){
        printf("Connection error\n");
        return 1;
    }

    printf("n=");
    scanf("%d", &n);


    n = htons(n);
    send(c, &n, sizeof(n), 0);
    n = ntohs(n);

    int i = 0;
    for(i = 0; i < n; ++i){
        printf("i=%d n=%d\n", i, n);
        // a = ntohs(a);
        printf("a=");
        scanf("%d", &a);
        a = htons(a);
        send(c, &a, sizeof(a), 0);
    }

    recv(c, &suma, sizeof(suma), 0);
    suma = ntohs(suma);
    printf("Suma = %d", suma);

    close(c);
}