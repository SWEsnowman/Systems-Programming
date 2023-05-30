#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#define MSGSIZE 60

//do we have to use atoi or does scanf work?
// also is it ok if we have any extra print out of the prompt even though the program shouldve terminated? - try to fix this
// does it have to be a seperate function or can it be in the main?
// CREATE THE MAKEFILE
int main (){
    int fd1[2];
    int fd2[2];
    pipe(fd1);
    pipe(fd2);
    int pid = fork();
    if (pid == 0){
        //child
        close(fd1[1]);
        close(fd2[0]);
        int guess;
        while (1){
            char buf[MSGSIZE];
            printf("Enter a number between 1 and 32: \n");
            scanf("%d", &guess);
            write(fd2[1], &guess, sizeof(guess));
            read(fd1[0], buf, MSGSIZE);
            printf("%s", buf);
            if (strcmp(buf, "Bingo!! \n") == 0){
                break;
            }
        }
        close(fd1[0]);
        close(fd2[1]);
        exit(0);

    }else {
        //parent
        int random;
        int passed_guess;
        close(fd1[0]);
        close(fd2[1]);
        int status;
        srand(time(NULL));
        random = rand() % 32 + 1;
        while (1){
            
            read(fd2[0], &passed_guess, sizeof(passed_guess));
            if (passed_guess == random){
                write(fd1[1], "Bingo!! \n", strlen("Bingo!! \n"));
                break;
                
            }
            else if (passed_guess > random){
                write(fd1[1], "Too high\n", strlen("Too high\n"));
            }
            else if (passed_guess < random){
                write(fd1[1], "Too low \n", strlen("Too low \n"));
            }
        }
        close(fd1[1]);
        close(fd2[0]);
        wait(&status);
        exit(0);
    }
}