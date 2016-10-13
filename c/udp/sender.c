/*
 * sender.c - multicasts "hello, world" to a multicast group once per second
 *
 * basic sample code (BSD) taken from here:
 * http://ntrg.cs.tcd.ie/undergrad/4ba2/multicast/antony/
 *
 */

#ifdef WIN32

#include <winsock2.h>
#include <stdio.h>

#else /* BSD */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

#endif

#define HELLO_PORT 12345
#define HELLO_GROUP "239.255.0.37"

#define HOSTNAMELEN 64
#define MESSAGELEN  64

main(int argc, char *argv[])
{
    struct sockaddr_in addr;
    int s;
    int count=0;
    char message[MESSAGELEN];
    char hostname[HOSTNAMELEN];

#ifdef WIN32
    /* Initialize Winsock */
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != NO_ERROR)
        printf("Error at WSAStartup()\n");
#endif

    /* create what looks like an ordinary UDP socket */

#ifdef WIN32
    if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET)
    {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
#else
    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket");
        exit(1);
    }
#endif

    /* set up destination address */
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(HELLO_GROUP);
    addr.sin_port = htons(HELLO_PORT);
    
    /* set up message to be multicast */
    gethostname(hostname, HOSTNAMELEN);

    /* now just sendto() our destination! */
    while (1)
    {
        sprintf(message, "%s multicasting: %d", hostname, count++);

        if (sendto(s, message, sizeof(message), 0, (struct sockaddr*) &addr, sizeof(addr)) < 0)
        {

#ifdef WIN32
            printf("Error at sendto(): %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
#else
            perror("sendto");
            exit(1);
#endif

        }

#ifdef WIN32
        Sleep(1000);
#else
        sleep(1);
#endif

    }
}
