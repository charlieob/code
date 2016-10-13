#include <stdio.h>

main()
{
char c1,c2,c3,c4;

for (c1='a'; c1<='z'; c1++)
for (c2='a'; c2<='z'; c2++)
for (c3='a'; c3<='z'; c3++)
for (c4='a'; c4<='z'; c4++)
{
printf("%c%c%c%c%camt\n",c1,c2,c3,c4,'e');
}
}
