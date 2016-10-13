#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

#define MAXBUF 50

main()
{
	key_t key=0xaa;
	int msgid;
	struct msqid_ds msqid_ds;
	struct msgbuf *msgp;
	long msgtype;
	int msgsz;
	int msgflg;
	int bytes;

	msgid = msgget(key,IPC_CREAT|0644);

	if (msgctl(msgid,IPC_STAT,&msqid_ds))
		printf("error in msgctl (errno=%d)\n",errno);

	msgp = (struct msgbuf *) malloc (sizeof(struct msgbuf) -
					sizeof(msgp->mtext) + MAXBUF);

	msgtype = 25;

	msgp->mtype = msgtype;
	sprintf (msgp->mtext, "hello there!*!");

	msgsz = strlen(msgp->mtext)+1;

	msgflg = IPC_NOWAIT;

	if (msgsnd(msgid,msgp,msgsz,msgflg))
		printf("error in msgsnd (errno=%d)\n",errno);

	msgp->mtext[0] = '\0';

	bytes = msgrcv(msgid,msgp,msgsz,msgtype,msgflg);
	if (bytes == -1)
		printf("error in msgrcv (errno=%d)\n",errno);
	else
		printf("message (%d bytes) received: %s\n",bytes,msgp->mtext);

	if (msgctl(msgid,IPC_RMID,0))
		printf("error in msgctl (errno=%d)\n",errno);
}

