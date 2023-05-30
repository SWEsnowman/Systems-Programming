#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>


int main(int argc, char *argv[]) {

    /*
      TODO:
      Use fork() and pipe(), but NOT popen().

      Create a pipe through which a child process will send two integers to 
      the parent. The parent will add the integers and print the result to 
      the terminal. 

      OPTIONAL:
      Make a second child, which will do the addition. In this case, the parent
      waits for the children to terminate. (See Lab 6.)
    */
    int pid;
    int num1 = 3;
    int num2 = 1;
    int num;
    int bytes;
    int bytes1;
    int fd[2];
    char line[4];
    if (pipe(fd) < 0) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    if (pid = fork() < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }else if (pid > 0){
      close(fd[1]);

      bytes = read(fd[0], &num, sizeof(int));
      //bytes1 = read(fd[0], &num2, sizeof(int));
      // while((bytes = read(fd[0], line, sizeof(line))) > 0) {
      //   read(fd[0], &num, bytes);
      // }
      write(STDOUT_FILENO, &num, sizeof(int));
      
    }else {
      close(fd[0]);
      int number1 = 5;
      int number2 = 10;
      int number3 = number1 + number2;
      write(fd[1], &number3, sizeof(int));
      //write(fd[1], &number2, sizeof(int));
    }
    printf("\n%d\n",num);
    
    return EXIT_SUCCESS;
}
