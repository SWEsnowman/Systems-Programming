//Olof Persson
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parser(FILE* input, FILE*output){
    char str[100];
    char* ptr; 
    long number; 
    while(fgets(str,100, input) != NULL){
        char *token = strtok(str, "\t");
        int count = 0;
        printf("hello");
        while(token != NULL){
            number = strtoul(token, &ptr, 10);
            if(count = 0){
                fwrite(&number, 4, 1,output);
                printf(0);
            } else if (count == 1){
                fwrite(&number, 2, 1, output);
                printf(1);
            }else if (count == 2){
                fwrite(&number, 1, 1, output);
                printf(2);
            }else if (count == 3){
                fwrite(&number, 8, 1, output);
                printf(3);
            }
            count++;
            token = strtok(NULL, "\t");
        }
    }
}

int main (int argc, char**argv){
    if(argc != 3){
        perror("Incorrect number of arguments: text2bin <input filename> <output filename>"); //perror???
        return 1;
    }
    FILE * inputfile = fopen(argv[1], "r");
    FILE * outputfile = fopen(argv[2], "w");
    parser(inputfile, outputfile);


    fclose(inputfile);
    fclose(outputfile);
    return 0;
}

