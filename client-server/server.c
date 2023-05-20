#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <string.h>

#define MAX 256
#define PORT 8000

void connection(int connfd)
{
    char buff[MAX];
    int n;
    char format_string[80]="%d,%d";
    int n1, n2;
    double mean;

    // infinite loop for chat
    while (1)
    {
        bzero(buff, MAX);

        // read the message from client and copy it in buffer
        if ((n = read(connfd, buff, sizeof(buff))) < 0)
        {
            perror("Error reading from socket");
        }

        if ((strncmp(buff, "q", 1)) == 0)
        {
            printf("Server Exit...\n");
            exit(0);
        }
        
        sscanf(buff, format_string, &n1, &n2);
        mean = (n1 + n2) / 2.0; 
        // print buffer which contains the client contents
        printf("Mean value is: %f, sum is: %d\n", mean, n1 + n2); 
        

        // and send that buffer to client
        if ((n = write(connfd, "I got your message", 18)) < 0)
        {
            perror("Error writing to socket");
        }
    }
}

int main(int argc, char *argv[])
{
    int sockfd, connfd, len;
    struct sockaddr_in serv_addr, cli_addr;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket creation failed...\n");
        exit(1);
    }
    else
        printf("Socket successfully created..\n");

    bzero((char *)&serv_addr, sizeof(serv_addr));

    // assign IP, PORT
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0)
    {
        perror("Socket bind failed...\n");
        exit(1);
    }
    else
        printf("Socket successfully binded..\n");

    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0)
    {
        perror("Listen failed...\n");
        exit(1);
    }
    else
        printf("Server listening..\n");

    len = sizeof(cli_addr);

    // Accept the data packet from client and verification
    connfd = accept(sockfd, (struct sockaddr *)&cli_addr, &len);
    if (connfd < 0)
    {
        perror("Server accept failed...\n");
        exit(1);
    }
    else
        printf("Server accept the client...\n");

    // Function for send and receive messages between client and server
    connection(connfd);

    // After chatting close the socket
    close(sockfd);
}