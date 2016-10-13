/*
 *  main.c
 *
 *  Created on: 18/10/2011
 *  Author: Cesar Carlos Ortiz Pantoja.
 */

#include <windows.h>
#include <stdio.h>

int exitCondition;

struct threadParams{
    int param1;
    int param2;
};

static DWORD WINAPI myFirstThread(void* threadParams)
{
    struct threadParams* params = (struct threadParams*)threadParams;

    while(exitCondition){
        printf("My Thread! Param1:%d, Param2:%d\n", params->param1, params->param2);
        fflush(stdout);
        Sleep(1000);
    }

    return 0;
}

int main(void){
    DWORD threadDescriptor;
    struct threadParams params1 = {1, 2};
    exitCondition = 1;

    _beginthreadex(
//    CreateThread(
        NULL,                   /* default security attributes.   */
        0,                      /* use default stack size.        */
        myFirstThread,          /* thread function name.          */
        (void*)&params1,        /* argument to thread function.   */
        0,                      /* use default creation flags.    */
        &threadDescriptor);     /* returns the thread identifier. */

    while(1){
        printf("Main Program!\n");
        fflush(stdout);
        Sleep(2000);
    }

    return 0;
}