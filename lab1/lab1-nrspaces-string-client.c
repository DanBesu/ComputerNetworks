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

    uint16_t len;

    c = socket(AF_INET, SOCK_STREAM, 0);
    if(c < 0){
        printf("socket error");
        return 1;
    }
    memset(&server, 0, sizeof(server));
    server.sin_port = htons(1234);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("192.168.100.9");
    
    if(connect(c, (struct sockaddr*) &server, sizeof(server)) < 0){
        printf("connection error");
        return 1;
    }
    char str[50];
    printf("Enter the string: ");
    fgets(str, 20, stdin);

    len = strlen(str);
    len = htons(len);

    send(c, &len, sizeof(len), 0);
    send(c, &str, sizeof(char) * len, 0);

    uint16_t nr;
    int response = recv(c, &nr, sizeof(nr), 0);
    if(response != sizeof(nr))
        printf("Error receiving the response\n");

    nr = ntohs(nr);
    printf("Number of spaces in the given string: %d", nr);
}
