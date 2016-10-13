#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SOCKET_PORT 2700

struct sockaddr_in ls_addr, to;

char msgbuf[2048];

main (argc, argv)
  int argc;
  char *argv[];
{
  int sx, s, count=1, totbytes=0, ofile;
  int size;

  if (argc != 2)
  {
    fprintf (stderr, "tcprecv <file>\n");
    exit (0);
  }

  if ((ofile = open (*++argv, O_CREAT|O_RDWR|O_TRUNC, 0660)) == -1)
  {
    fprintf (stderr, "can't open file '%s'\n", *argv);
    exit (errno);
  }

  if ((sx = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    fprintf (stderr, "can't open socket (%d)\n", errno);
    close (ofile);
    exit (errno);
  }

  ls_addr.sin_family = AF_INET;
  ls_addr.sin_port = htons (SOCKET_PORT);
  ls_addr.sin_addr.s_addr = 0;

  if (bind (sx, (struct sockaddr*) &ls_addr, sizeof(ls_addr)) == -1)
  {
    fprintf (stderr, "can't bind socket\n");
    close (sx);
    close (ofile);
    exit (errno);
  }

  if (listen (sx, 1) < 0)
  {
    fprintf (stderr, "listen failed\n");
    close (sx);
    close (ofile);
    exit (errno);
  }

  size = sizeof (struct sockaddr_in);

  if ((s = accept (sx, (struct sockaddr*) &to, &size)) < 0)
  {
    fprintf (stderr, "can't accept");
    close (sx);
    close (ofile);
    exit (errno);
  }

  close (sx);

  printf ("connected to %d,%s\n", to.sin_port, inet_ntoa (to.sin_addr));

  while (count)
  {
    if ((count = read (s, msgbuf, sizeof(msgbuf))) < 0)
    {
      fprintf (stderr, "can't receive (cnt = %d)\n", count);
      close (s);
      close (ofile);
      exit (errno);
    }
    else if (count == 0)
      break;
    else
    {
      printf ("bytes received: %d\n", count);

      if (write (ofile, msgbuf, count) != count)
      {
        fprintf (stderr, "can't write output\n");
        close (s);
        close (ofile);
        exit (errno);
      }
      totbytes += count;
    }
  }

  close (ofile);
  close (s);
  printf ("read %d bytes\n", totbytes);
  exit (0);
}
