#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SOCKET_PORT 6692

struct sockaddr_in ls_addr, to;

char msgbuf[2048];

main (argc, argv)
  int argc;
  char *argv[];
{
  int sx, s, size, i, n, done;
  unsigned char msg[100];

  if (argc != 1)
  {
    fprintf (stderr, "chatsrv");
    exit (0);
  }

  if ((sx = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    fprintf (stderr, "can't open socket (%d)\n", errno);
    exit (errno);
  }

  ls_addr.sin_family = AF_INET;
  ls_addr.sin_port = htons (SOCKET_PORT);
  ls_addr.sin_addr.s_addr = 0;

  if (bind (sx, (struct sockaddr*) &ls_addr, sizeof(ls_addr)) == -1)
  {
    fprintf (stderr, "can't bind socket (%d)\n", errno);
    close (sx);
    exit (errno);
  }

  if (listen (sx, 1) < 0)
  {
    fprintf (stderr, "listen failed\n");
    close (sx);
    exit (errno);
  }

  size = sizeof (struct sockaddr_in);

  if ((s = accept (sx, (struct sockaddr*) &to, &size)) < 0)
  {
    fprintf (stderr, "can't accept");
    close (sx);
    exit (errno);
  }

  printf ("connected to %d,%s\n", to.sin_port, inet_ntoa (to.sin_addr));

  /* 2 sync bytes */
  msg[0] = 0x74;
  msg[1] = 0x85;

  /* 2-byte message id */
  msg[2] = 0x10;
  msg[3] = 0x01;

  /* 4-byte length of body (=7) */
  msg[4] = 0;
  msg[5] = 0;
  msg[6] = 0;
  msg[7] = 7;

  /* cConnect = 0x02 */
  msg[8] = 0x02;

  /* SlotNr=2 encoded in 16 bits */
  msg[9]  = 0x01;
  msg[10] = 0;
  msg[11] = 0x02;

  /* Mode=1 encoded in 16 bits */
  msg[12] = 0x01;
  msg[13] = 0;
  msg[14] = 1;

  write (s, msg, 8+7);

  done = 0;
  while (!done)
  {
    n = read (s, msgbuf, sizeof(msgbuf));
    if (n <= 0)
      done = 1;
    else
    {
      printf ("received %d characters: 0x", n);
      for (i=0; i<n; i++)
        printf ("%02x", (unsigned char)msgbuf[i]);
      putchar ('\n');
    }
  }

  printf ("disconnected (%d)\n", n);

  shutdown (s, 2);
  shutdown (sx, 2);
  close (s);
  close (sx);

  exit (0);
}
