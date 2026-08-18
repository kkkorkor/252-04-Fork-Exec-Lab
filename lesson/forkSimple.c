#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
    pid_t pid;
    char *msg;
    int n;

    printf(“fork program starting\n”);
    pid = fork(); //child more than 1 use loop to fork
    switch(pid)
    {
    case -1: _exit(1);
    case 0: msg = “Child!\n”; 
            n = 45; 
            break;
    default: msg = “Parent!\n”;
            n = 3; 
            break;
    }

    for (; n>0; n--){ 
        puts(msg); sleep(1); 
    }
    _exit(0);
}