#define _GNU_SOURCE

#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int sockfd, n;
    // struct sockaddr_in serv_addr;
    // struct hostent *server;

    char buffer[256];

    if (argc < 3)
    {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }

    // portno = atoi(argv[2]);

    /* Create a socket point */
    //sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // if (sockfd < 0)
    // {
    //     perror("ERROR opening socket");
    //     exit(1);
    // }

    // server = gethostbyname(argv[1]);

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;       // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP
    struct addrinfo *result;
    int rval = getaddrinfo(argv[1], argv[2], &hints, &result);

    if (rval != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rval));
        exit(EXIT_FAILURE);
    }

    /*
    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }
    */

    struct addrinfo *p;
    for (p = result; p != NULL; p = p->ai_next)
    {
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

        if (sockfd < 0)
        {
            continue;
        }
        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == 0)
        {
            break;
        }
        close(sockfd);
    }
    // p points to the addrinfo that connected to the host/server

    if (p == NULL) {
        // fprintf -- could not connect to host
        fprintf(stderr, "Could not connect to host %s:%s\n", argv[1], argv[2]);
        freeaddrinfo(result);
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(result);

    /*
    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    

    //Now connect to the server 
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("ERROR connecting");
        exit(1);
    }
    */

    /* Now ask for a message from the user, this message
     * will be read by server
     */

    printf("Please enter the message: ");
    memset(buffer, 0, 256);
    fgets(buffer, 255, stdin);

    /* Send message to the server */
    n = write(sockfd, buffer, strlen(buffer));

    if (n < 0)
    {
        perror("ERROR writing to socket");
        exit(1);
    }

    /* Now read server response */
    memset(buffer, 0, 256);
    n = read(sockfd, buffer, 255);

    if (n < 0)
    {
        perror("ERROR reading from socket");
        exit(1);
    }

    printf("%s\n", buffer);
    return 0;
}