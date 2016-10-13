#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

typedef struct shm_t {
	char a;
	char b;
	int c[100];
} shm_t;

main()
{
	key_t key=0xaa;
	int shmid;
	struct shmid_ds shmid_ds;
	shm_t *p;
	int i;

	shmid = shmget(key,sizeof(shm_t),IPC_CREAT|0666);
	if (shmid == -1)
		printf("error in shmget (errno=%d)\n",errno);

	p = (shm_t *) shmat(shmid,(char *)0,0);
        if (p == (shm_t *) -1)
		printf("error in shmat (errno=%d)\n",errno);

	printf ("p=%x\n",p);

	p->a = 'a';
	p->b = 'b';
	for (i=0; i<100; i++)
		p->c[i] = i+1000;

	printf ("%d...%d\n",p->c[0]-1000,p->c[99]-1000);

	if (shmctl(shmid,IPC_STAT,&shmid_ds))
		printf("error in shmctl (errno=%d)\n",errno);

	if (shmctl(shmid,IPC_RMID,(struct shmid_ds *)0))
		printf("error in shmctl (errno=%d)\n",errno);

	if (shmdt(p))
		printf("error in shmdt (errno=%d)\n",errno);
}

