#include <stdio.h>

main ()
{
  unsigned char s;
  int i_n,i_p;
  char c_n,c_p;
  unsigned char uc_p;

  s = sizeof(int);
  i_n = 1<<(8*s-1);
  i_p = ~i_n;
  printf ("sizeof(int)=%d\n",s);
  printf ("range is %d...%d\n", i_n, i_p);
  s = sizeof(char);
  c_n = 1<<(8*s-1);
  c_p = ~c_n;
  printf ("sizeof(char)=%d\n",s);
  printf ("range is %d...%d\n", c_n, c_p);
  s = sizeof(unsigned char);
  uc_p = ~0;
  printf ("sizeof(unsigned char)=%d\n",s);
  printf ("range is %d...%d\n", 	0, uc_p);
  s = sizeof(long);
  printf ("sizeof(long)=%d\n",s);
  s = sizeof(unsigned long);
  printf ("sizeof(unsigned long)=%d\n",s);
  s = sizeof(short);
  printf ("sizeof(short)=%d\n",s);
  s = sizeof(unsigned short);
  printf ("sizeof(unsigned short)=%d\n",s);
  s = sizeof(float);
  printf ("sizeof(float)=%d\n",s);
  s = sizeof(double);
  printf ("sizeof(double)=%d\n",s);
}
