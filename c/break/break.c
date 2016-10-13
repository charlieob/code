#include <stdio.h>

main()
{
	int i;
	
	i = 10;
	while (i > 0) {
		printf("i = %d\n", i);
		if (i == 5) {
			printf("breaking now\n");
			break;
		}
		i--;
	}
	
	printf("while section done\n");
	
	i = 10;
	do {
		printf("i = %d\n", i);
		if (i == 5) {
			printf("breaking now\n");
			break;
		}
		i--;
	} while (i > 0);
	
	printf("do section done\n");
	
	for (i=0; i<10; i++) {
		printf("i = %d\n", i);
		if (i == 5) {
			printf("breaking now\n");
			break;
		}
	}
	
	printf("for section done\n");
}