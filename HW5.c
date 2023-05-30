#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <signal.h>
#include <sys/signal.h>
#include <unistd.h>
#include <wait.h>



//what kind of header file? can it not all be in the same file? YES make it so that i have function outside main function

//USE KILLPG() ON ALL THE EXISTS???????????????
void sigint_handler(int signo){
   printf("\nShell cannot be terminated like this.\n");
}

void sigchld_handler(int signo){
    printf("Child process exited.\n");
}

void child_handler2(int sig){ 
    int olderrno = errno;
    pid_t pid;
    while ((pid = wait(NULL)) > 0) {
        printf("Handler reaps child %d\n", (int)pid);
    }
}
// Path: HW5.h
int minishell(){
    char * home_dir;
    int count = 0;
    if(signal(SIGINT, sigint_handler) == SIG_ERR){
        printf("Error: Cannot register signal handler. %s.\n", strerror(errno));
        exit(1);
    }
    if(signal(SIGCHLD, sigchld_handler) == SIG_ERR){
        printf("Error: Cannot register signal handler. %s.\n", strerror(errno));
        exit(1);
    }
    while(true){
        char * path;
        char *username = getlogin();
        if((path = getcwd(NULL,0)) == NULL){
            printf("Error: Cannot get current working directory. %s.\n", strerror(errno));
            exit(1);
        }
        printf("minishell:%s:%s$ ", username,path);
        

        char * line;
        size_t len = 0;
        int fail = getline(&line, &len, stdin);
        if(fail == -1){
            printf("Error: Failed to read from stdin. %s.\n", strerror(errno));
            exit(1);
        }
        char * token = strtok(line, " ");
        char * args[10];
        int i = 0;
        while(token != NULL){
            args[i] = token;
            token = strtok(NULL, " ");
            i++;
        }
        args[i] = NULL;
        if(strcmp(args[0], "exit") == 0 || strcmp(args[0], "exit\n") == 0){
            if (args[1] != NULL && strcmp(args[1], "\n") != 0){
                printf("exit: Too many arguments supplied\n");
                exit(1);
            }
            exit(0);
        }
        else if(strcmp(args[0], "cd") == 0 || strcmp(args[0], "cd\n") == 0){
            
            if((args[1] == NULL) || strcmp(args[1],"~") == 0 || strcmp(args[1],"~\n") == 0 || strcmp(args[1],"\n") == 0){
                uid_t uid = getuid();
                struct passwd * pass;
                if((pass = getpwuid(uid)) == NULL){
                    printf("Error: Cannot get passwd entry. %s.\n", strerror(errno));
                    exit(1);
                }
                home_dir = pass->pw_dir;
                chdir(home_dir);
                
            }
            else if(args[2] != NULL){
                printf("Error: Too many arguments to cd.\n");
                exit(1);
            }
            else{
                args[1][strlen(args[1])-1] = '\0';

                int correct = chdir(args[1]);
                if(correct == -1){
                    printf("Error: Cannot change directory to %s. %s.\n", args[1], strerror(errno));
                    exit(1);
                }
            }
        }
        else{
            pid_t pid;
            int status;
            if(strcmp(args[i-1],"&\n") == 0){
                pid = fork();
                if(pid == -1){
                    printf("Error: fork() failed. %s.\n", strerror(errno));
                    exit(1);
                }
                if(pid == 0){
                    args[i-1][strlen(args[i-1])-1] = '\0';
                    char * background_args[10];
                    for (int j = 0; j < i-1; j++){
                        background_args[j] = args[j];
                    }
                    printf("pid: %d cmd: %s\n", getpid(), line);
                    int failed = execvp(args[0], background_args); 
                    if(failed == -1){
                        printf("Error: exec() failed. %s.\n", strerror(errno));
                        exit(1);
                    }

                }else{
                    while(true){
                    int err = waitpid(-1, &status, WNOHANG); // TODO wait for background process using waitpid
                        if(err == -1){
                            printf("Error: wait() failed. %s.\n", strerror(errno));
                            
                            exit(1);
                        }else if(err == 0){
                            continue;
                        }else{
                            printf("pid %d done\n", err);
                            break;
                        }
                    }
                }
            }else{
                pid = fork();
                if(pid == -1){
                    printf("Error: fork() failed. %s.\n", strerror(errno));
                    exit(1);
                }
                if(pid == 0){
                    args[i-1][strlen(args[i-1])-1] = '\0';
                    int failed = execvp(args[0], args);
                    if(failed == -1){
                        printf("Error: exec() failed. %s.\n", strerror(errno));
                        exit(1);
                    }
                }else{
                    int err = waitpid(-1, &status, WCONTINUED);
                    if(err == -1){
                        printf("Error: wait() failed. %s.\n", strerror(errno));
                        exit(1);
                    }if(err == 0){
                        continue;
                    }   
                }
            }
        }
    }
}

int main(int argc, char * argv){
    minishell();
    killpg(getpid(), SIGTERM); // TODO kill all child processes
    return 0;
}