#include <stdio.h>
#include <windows.h>
#include <process.h>

CRITICAL_SECTION CriticalSection;

/*
-- a very simple queue data structure
-- done as a global variable, with apologies to the purists
-- has operations add, remove and test for emptiness
*/
typedef struct q_item {
	int data;
	struct q_item *next;
} q_item;
q_item *queue=NULL; 	// that's it initialised

void q_add(int n)
{
	q_item *q = malloc(sizeof(q_item));
	q->data = n;
	q->next = NULL;
	
	EnterCriticalSection(&CriticalSection);
	q_item *qp = queue;
	if (qp == NULL) {
		queue = q;
	}
	else {
		while (qp->next != NULL) qp = qp->next;
		qp->next = q;
	}
	LeaveCriticalSection(&CriticalSection);
}

/*
-- must check that the queue is not empty before calling this...
*/
void q_remove(int *n)
{
	q_item *q;
	
	EnterCriticalSection(&CriticalSection);
	q = queue;
	*n = q->data;
	queue = q->next;
	LeaveCriticalSection(&CriticalSection);
	free(q);
}

int q_empty(void)
{
	return (queue == NULL);
}

unsigned char threadrunning = 0;
void myThread(void* dummy)
{
	char s[80];
	int num;

    while (1) {
		printf("enter number here> ");
		num = atoi(fgets(s, 80, stdin));
		if (num == -1) {
			threadrunning = 0;
			break;
		}
		q_add(num);
    }
}

int main(void)
{
#define DATASIZE 100
	int input_data[DATASIZE];
	int n=0;
	int num;

	InitializeCriticalSectionAndSpinCount(&CriticalSection, 0x00000400);

	threadrunning = 1;
    _beginthread(myThread, 0, NULL);

	// program is for illustrative purposes only
	// no check done here that boundary of input_data[] is not overwritten
    while (1) {
		while (!q_empty()) {
			q_remove(&num);
			input_data[n++] = num;
		}
		if (!threadrunning) break;
        Sleep(500);
    }
	
	int i;
	printf("numbers entered: ");
	for (i=0; i<n; i++) {
		printf("%d ", input_data[i]);
	}
	printf("\n");

	DeleteCriticalSection(&CriticalSection);
    return 0;
}