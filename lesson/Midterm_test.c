#include <stdio.h>
#include <unistd.h> //for use fork
#include <sys/types.h>
#include <sys/wait.h> //if use wait, need this library

int main(){
    pid_t pid[3];
    char *msg;

    for(int i=0; i<3; i++){
        pid[i] = fork(); 
        
        switch(pid[i]){
                case 0: msg = "Child Process";
                        puts(msg);

                        //execl("/bin/ls", "ls", NULL);
                        //perror("execl");
                        _exit(0);
                        fflush(stdout); //for clear buffer
                        //write(); //for clear buffer
                case -1: msg = "Fork fail";
                        puts(msg);
                        break;
                default: msg = "Parent Process";
                        wait(NULL); 
                        // //--> If want child to finish the process first, let parent wait()
                        puts(msg); 
                        break;
            }        
    }



    _exit(0);
}