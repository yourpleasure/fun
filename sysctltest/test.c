#include <unistd.h>
#include <sys/syscall.h>
#include "sysctl-exam.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#define SIZE(x) sizeof(x)/sizeof(x[0])
#define ISNAMESZ 100

int oldmyint;
size_t oldmyintlen;
int newmyint;
size_t newmyintlen;
char oldmystring[MY_MAX_SIZE];
size_t oldmystringlen;
char newmystring[MY_MAX_SIZE];
size_t newmystringlen;

int myintctl[] = {MY_ROOT, MY_INT_EXAM};
int mystringctl[] = {MY_ROOT, MY_STRING_EXAM};

int main(int argc, char *argv[])
{
    if(argc < 2){
        oldmyintlen = sizeof(int);
        if(syscall(SYS__sysctl, myintctl, SIZE(myintctl), &oldmyint, &oldmyintlen, 0, 0)){
            perror("sysctl");
            exit(-1);
        }
        else{
            printf("mysysctl.myint = %d\n", oldmyint);
        }

        oldmystringlen = MY_MAX_SIZE;
        if(syscall(SYS__sysctl, mystringctl, SIZE(mystringctl), oldmystring, &oldmystringlen, 0, 0)){
            perror("sysctl");
            exit(-1);
        }
        else{
            printf("mysysctl.mystring = \"%s\"\n", oldmystring);
        }
    }
    else if(argc != 3){
        printf("Usage:\n");
        printf("\tsysctl-exam-user\n");
        printf("Or\n");
        printf("\tsysctl-exam-user aint astring\n");
    }
    else{
        newmyint = atoi(argv[1]);
        newmyintlen = sizeof(int);
        oldmyintlen = sizeof(int);

        strcpy(newmystring, argv[2]);
        newmystringlen = strlen(newmystring);
        oldmystringlen = MY_MAX_SIZE;

        if(syscall(SYS__sysctl, myintctl, SIZE(myintctl), &oldmyint, &oldmyintlen, &newmyint, newmyintlen)){
            perror("sysctl");
            exit(-1);
        }
        else{
            printf("old value: mysysctl.myint = %d\n", oldmyint);
            printf("new value: mysysctl.myint = %d\n", newmyint);
        }

        if(syscall(SYS__sysctl, mystringctl, SIZE(mystringctl), oldmystring, &oldmystringlen, newmystring, newmystringlen)){
            perror("sysctl");
            exit(-1);
        }
        else{
            printf("old value: mysysctl.mystring \"%s\"\n", oldmystring);
            printf("new value: mysysctl.mystring \"%s\"\n", newmystring);
        }
    }

    exit(0);
}
