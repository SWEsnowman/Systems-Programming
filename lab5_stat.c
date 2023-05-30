#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int perms[] = {0000400, 0000200, 0000100,
               0000040, 0000020, 0000010,
               0000004, 0000002, 0000001}; //the sys/stat.h permission keys didnt work

void display_usage(char *progname) {
    printf("Usage: %s <filename>\n", progname);
}

/**
 * TODO
 * Returns a string (pointer to char array) containing the permissions of the
 * file referenced in statbuf.
 * Allocates enough space on the heap to hold the 10 printable characters.
 * The first char is always a - (dash), since all files must be regular files.
 * The remaining 9 characters represent the permissions of user (owner), group,
 * and others: r, w, x, or -.
 */
char* permission_string(struct stat *statbuf) {
    char p[10];
    p[0] = '-';
    char *ret = (char *) malloc(11);
    int permission_valid;
    for(int i = 0; i < 9; i += 3){
        for(int j = i; j < i+3; j++){
            permission_valid = statbuf->st_mode & perms[j];
            if(permission_valid) {
                if(j == i){
                    p[i+1] = 'r'; 
                }else if (j == i+1){
                    p[i+2] = 'w';
                }else{
                    p[i+3] = 'x';
                }
                
            }
            else {
                p[j] = '-';
            }
        
        }

    }
    strcpy(ret, p);
    return ret;
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        display_usage(argv[0]);
        return EXIT_FAILURE;
    }

    struct stat statbuf;
    if (stat(argv[1], &statbuf) < 0) {
        fprintf(stderr, "Error: Cannot stat '%s'. %s.\n", argv[1],
                strerror(errno));
        return EXIT_FAILURE;
    }

    /* TODO
     * If the argument supplied to this program is not a regular file,
     * print out an error message to standard error and return EXIT_FAILURE.
     * For example:
     * $ ./permstat iamdir
     * Error: 'iamdir' is not a regular file.
     */
    if(S_ISDIR((&statbuf)->st_mode)){
        fprintf(stderr,"Error: %s is not a regular file\n", argv[1]);
        strerror(errno);
        return EXIT_FAILURE;
    }
    char *perms = permission_string(&statbuf);
    printf("Permissions: %s\n", perms);
    free(perms);

    return EXIT_SUCCESS;
}
