#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>


int main(int argc, char* argv[]) { 
    int port = atoi(argv[2]);
    char* address = argv[1];
    int sock = 0, valread = 0;
    struct sockaddr_in serv_addr; 
    printf("Starting client socket on addr of %s, port of %d\n", address, port);
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
    
    // convert hostname to ip
    char ip_addr[257];
    struct hostent* hnet = gethostbyname(address);
    strcpy(ip_addr, inet_ntoa(*((struct in_addr*) hnet -> h_addr_list[0])));
    printf("ip address is %s\n",ip_addr);

    // connect
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    
    // convert ipv4/6 to binary
    if (inet_pton(AF_INET, ip_addr, &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address or Address not supported\n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    printf(">");
    unsigned char q[257];
    while (fgets(q, 257, stdin)) {
        char buf[257];
        int len = strlen(q);
        if (len < 1) { 
            printf(">");
            continue; 
        }
        unsigned char length = len & 0xFF;
        unsigned char query[257];
        query[0] = length;
        query[1] = '\0';
        q[len] = '\0';
        strcat(query, q);
        send(sock, query, strlen(query), 0);
        valread = read(sock, buf, 257);
        if (!strncmp(buf+1, "quit", 4)) {
            break;
        }
        printf("%s\n", buf+1);
        printf(">");
        memset(buf, 0, 257);
    }
    close(sock);
    return 0; 
}