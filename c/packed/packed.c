#include <stdio.h>
#include <inttypes.h>	// for C99 types such as uint16_t

char bytearray[] = {0,1,2,3,4,5};

typedef char uint8;
typedef unsigned short uint16;

struct mystruct_t
{
	uint16 a;
	uint8  b;
	uint16 c;
};

#define PACKSTRUCT( decl ) decl __attribute__((__packed__,gcc_struct))

PACKSTRUCT(struct my_packed_struct_t
//struct __attribute__ ((__packed__, gcc_struct)) my_packed_struct_t
{
	uint16 a;
	uint8  b;
	uint16 c;
});

main ()
{

#ifdef __GNUC__
	printf("__GNUC__ is defined\n");
#endif

#ifdef _WIN32
	printf("_WIN32 is defined\n");
#endif

	printf("sizeof(char) = %d\n", sizeof(char));
	printf("sizeof(int) = %d\n", sizeof(int));
	printf("sizeof(unsigned int) = %d\n", sizeof(unsigned int));
	printf("sizeof(unsigned short) = %d\n", sizeof(unsigned short));
	printf("sizeof(uint8) = %d\n", sizeof(uint8));
	printf("sizeof(uint16) = %d\n", sizeof(uint16));
	printf("sizeof(uint8_t) = %d\n", sizeof(uint8_t));
	printf("sizeof(uint16_t) = %d\n", sizeof(uint16_t));

	char *p = bytearray;
	int i;
	
	for (i=0; i<6; i++)
		printf("%d ", p[i]);
	printf("\n");
	
	struct mystruct_t* mystruct;
	mystruct = (struct mystruct_t*) bytearray;

	printf("a = %04x\n", mystruct->a);	// should come out as "a = 0100" on Windows due to little-endian Intel architecture
	printf("b = %02x\n", mystruct->b);	// b = 02
	printf("c = %04x\n", mystruct->c);	// should come out as "c = 0504" due to word alignment, i.e. the 3 is missed

	struct my_packed_struct_t* my_packed_struct;
	my_packed_struct = (struct my_packed_struct_t*) bytearray;

	printf("a = %04x\n", my_packed_struct->a);	// should come out as "a = 0100" on Windows due to little-endian Intel architecture
	printf("b = %02x\n", my_packed_struct->b);	// b = 02
	printf("c = %04x\n", my_packed_struct->c);	// should come out as "c = 0403" due to the packing

	
	
}

