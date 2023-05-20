#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()

#define MAX 256
#define PORT 8000

void connection(int sockfd)
{
    char buff[MAX];
    int n;
    while (1)
    {
        bzero(buff, sizeof(buff));
        printf("Please, write two integer numbers, separated by commas (,), or q to quit\n");
        fflush(stdout);
        fgets(buff, 255, stdin);

        if ((n = write(sockfd, buff, sizeof(buff))) < 0)
        {
            perror("ERROR writing to socket");
        }

        if ((strncmp(buff, "q", 1)) == 0)
        {
            printf("Client Exit...\n");
            exit(0);
        }

        bzero(buff, sizeof(buff));

        if ((n = read(sockfd, buff, sizeof(buff))) < 0)
        {
            perror("ERROR reading from socket");
        }

        printf("From Server : %s\n", buff);
    }
}

int main()
{
    int sockfd, connfd;
    struct sockaddr_in serv_addr, cli_addr;

    // socket create and verification
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket creation failed...\n");
        exit(1);
    }
    else
        printf("Socket successfully created..\n");

    bzero(&serv_addr, sizeof(serv_addr));

    // assign IP, PORT
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("172.20.10.4");
    serv_addr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if ((connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) != 0)
    {
        perror("Connection with the server failed...\n");
        exit(1);
    }
    else
        printf("Connected to the server..\n");

    connection(sockfd);

    // close the socket
    close(sockfd);
}
