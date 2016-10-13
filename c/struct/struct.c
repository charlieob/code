#include <stdio.h>

typedef struct myStruct {
	int n;
	char s[10];
} myStruct;

typedef char string[10];

main()
{
	myStruct s1,s2;
	string str1,str2;
	int i;

	s1.n = 5;
	sprintf(s1.s, "hello");
	s1.s[6] = '6';
	s1.s[7] = '7';
	s1.s[8] = 'x';
	s1.s[9] = 'y';

	s2 = s1;

	printf ("s2 = %d,",s2.n);
	for (i=0; i<10; i++)
		printf("%c",s2.s[i]);
	printf ("\n");

/*
	sprintf(str1,"string");
	str2 = str1;
	printf ("str2 = %s\n", str2);
*/

}
