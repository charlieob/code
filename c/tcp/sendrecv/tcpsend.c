#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <netdb.h>
#include <signal.h>

#define RETRY_COUNT 10
#define SOCKET_PORT 2700

struct sockaddr_in ls_addr;
char msgbuf[2048];

void onpipe ();

main (argc, argv)
  int argc;
  char *argv[];
{
  int count=RETRY_COUNT, s, ifile, wc;
  int totbytes=0;
  unsigned short *usp;
  struct hostent *host;
  char optval[50];
  char optlen;
  int mask, oldmask;

  if (argc != 3)
  {
    fprintf (stderr, "tcpsend <host> <file>\n");
    exit (0);
  }

  if ((host = gethostbyname (*++argv)) == NULL)
  {
    fprintf (stderr, "don't know host '%s'\n", *argv);
    endhostent ();
    exit (errno);
  }

  if ((ifile = open (*++argv, O_RDONLY)) == -1)
  {
    fprintf (stderr, "can't open file '%s'\n", *argv);
    endhostent ();
    exit (errno);
  }

  signal (SIGPIPE, onpipe);

/*
  mask = sigmask (SIGPIPE);
  oldmask = sigblock (mask);
*/

  while (1)
  {
    if ((s = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      fprintf (stderr, "can't open socket (%d)\n", errno);
      endhostent ();
      close (s);
      close (ifile);
      exit (errno);
    }

    ls_addr.sin_family = host->h_addrtype;
    ls_addr.sin_port = htons (SOCKET_PORT);
    memcpy (&ls_addr.sin_addr.s_addr, host->h_addr, host->h_length);

    if (connect (s, (struct sockaddr*) &ls_addr, sizeof(ls_addr)) == -1)
    {
      fprintf (stderr, "connect failed\n");
      if (--count)
      {
        close (s);
        fprintf (stderr, "retry connect %d\n", RETRY_COUNT - count);
        sleep (1);
        continue;
      }

      close (s);
      close (ifile);
      endhostent ();
      exit (errno);
    }
    break;
  }

  endhostent ();
  printf ("Connection established with %d,%s\n",
		ls_addr.sin_port,
		inet_ntoa (ls_addr.sin_addr));
  printf ("about to send file '%s' ...\n", *argv);
  fflush (stdout);

/*
  optlen = sizeof(optval);
  if (!getsockopt (s, SOL_SOCKET, SO_KEEPALIVE, optval, &optlen))
    fprintf (stderr, "getsockopt() failed\n");

  usp = (unsigned short *) optval;
  *usp = 1;
  optlen = sizeof(*usp);
  if (!setsockopt (s, SOL_SOCKET, SO_KEEPALIVE, optval, &optlen))
    fprintf (stderr, "setsockopt() failed\n");

  optlen = sizeof(optval);
  if (!getsockopt (s, SOL_SOCKET, SO_KEEPALIVE, optval, &optlen))
    fprintf (stderr, "getsockopt() failed\n");
*/

/*
  optlen = sizeof(optval);
  if (!getsockopt (s, SOL_SOCKET, SO_DEBUG, optval, &optlen))
    fprintf (stderr, "getsockopt() failed\n");

  usp = (unsigned short *) optval;
  *usp = 1;
  optlen = sizeof(*usp);
  if (!setsockopt (s, SOL_SOCKET, SO_DEBUG, optval, &optlen))
    fprintf (stderr, "setsockopt() failed\n");

  optlen = sizeof(optval);
  if (!getsockopt (s, SOL_SOCKET, SO_DEBUG, optval, &optlen))
    fprintf (stderr, "getsockopt() failed\n");
*/

  sleep (5);

  count = read (ifile, msgbuf, 10);
  wc = write (s, msgbuf, count);
  printf ("bytes sent: %d\n", wc);
  if (wc != count)
  {
    fprintf (stderr, "socket write error\n");
    close (s);
    close (ifile);
    exit (errno);
  }
  totbytes += count;

  count = read (ifile, msgbuf, 20);
  wc = write (s, msgbuf, count);
  printf ("bytes sent: %d\n", wc);
  if (wc != count)
  {
    fprintf (stderr, "socket write error\n");
    close (s);
    close (ifile);
    exit (errno);
  }
  totbytes += count;

  while ((count = read (ifile, msgbuf, sizeof(msgbuf))) > 0)
  {
    wc = write (s, msgbuf, count);
    printf ("bytes sent: %d\n", wc);
    if (wc != count)
    {
      fprintf (stderr, "socket write error\n");
      close (s);
      close (ifile);
      exit (errno);
    }
    totbytes += count;
  }

  if (count != 0)
  {
    fprintf (stderr, "read error on file\n");
    close (s);
    close (ifile);
    exit (errno);
  }

  close (ifile);

/*
  sprintf (msgbuf, "data\n");
  for (count=0; count<1000; count++)
  {
    wc = write(s, msgbuf, strlen(msgbuf));
    printf ("bytes sent: %d\n", wc);
    if (wc != strlen(msgbuf))
    {
      fprintf (stderr, "failed (%d)\n", errno);
      close (s);
      exit (errno);
    }
  }
*/

  close (s);
  printf ("sent %u bytes\n", totbytes);
  exit (0);
}

void onpipe ()
{
  fprintf (stderr, "signal SIGPIPE received\n");
}
