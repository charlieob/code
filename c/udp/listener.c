/*
 * listener.c - joins a multicast group and echoes all data it receives from
 *              the group to its stdout...
 *
 * basic sample code (BSD) taken from here:
 * http://ntrg.cs.tcd.ie/undergrad/4ba2/multicast/antony/
 *
 */

#ifdef WIN32

#include <winsock2.h>
#include <ws2tcpip.h>
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
#define MSGBUFSIZE 256

main(int argc, char *argv[])
{
    struct sockaddr_in addr;
    int nbytes, addrlen;
    struct ip_mreq mreq;
    char msgbuf[MSGBUFSIZE];
    u_int yes=1;
    int s;

#ifdef WIN32
    /* Initialize Winsock */
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != NO_ERROR)
        printf("Error at WSAStartup()\n");
#endif

    /* create what looks like an ordinary UDP socket */
    if ((s=socket(AF_INET,SOCK_DGRAM,0)) < 0)
    {

#ifdef WIN32
        printf("Error at socket(): %ld\n", WSAGetLastError());
        WSACleanup();
        return(1);
#else
        perror("socket");
        exit(1);
#endif

    }

#ifdef NOTSUREIFYOUNEEDTHIS
    /* allow multiple sockets to use the same PORT number */
    if (setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes)) < 0)
    {

#ifdef WIN32
        printf("Error at setsockopt(): %ld\n", WSAGetLastError());
        WSACleanup();
        return(1);
#else
        perror("Reusing ADDR failed");
        exit(1);
#endif

    }
#endif
    
    /* set up destination address */
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(HELLO_PORT);
    
    /* bind to receive address */
    if (bind(s, (struct sockaddr *) &addr, sizeof(addr)) < 0)
    {

#ifdef WIN32
        printf("Error at bind(): %ld\n", WSAGetLastError());
        WSACleanup();
        return(1);
#else
        perror("bind");
        exit(1);
#endif

    }
    
    /* use setsockopt() to request that the kernel join a multicast group */
    mreq.imr_multiaddr.s_addr = inet_addr(HELLO_GROUP);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);

#ifdef WIN32
    if (setsockopt(s, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*) &mreq, sizeof(mreq)) < 0)
    {
        printf("Error at setsockopt(): %ld\n", WSAGetLastError());
        WSACleanup();
        return(1);
#else
    if (setsockopt(s, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0)
    {
        perror("setsockopt");
        exit(1);
#endif

    }
    
    /* now just enter a read-print loop */
    while (1) {
        addrlen = sizeof(addr);
        if ((nbytes = recvfrom(s, msgbuf, MSGBUFSIZE, 0, (struct sockaddr*) &addr,&addrlen)) < 0)
        {

#ifdef WIN32
            printf("Error at bind(): %ld\n", WSAGetLastError());
            WSACleanup();
            return(1);
#else
            perror("recvfrom");
            exit(1);
#endif

        }

        puts(msgbuf);
    }
}
