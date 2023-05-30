//#define _POSIX_SOURCE
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
int write_perm =  S_IWUSR;
struct max_return_values {
    int write;
    int nonwrite;
    char * write_name;
    char * nonwrite_name;
    long disk_size;
}
typedef max_return_values;

struct max_return_values read_through(char * dirname){
    DIR * direct;
    struct stat stats;
    if(!(direct = opendir(dirname))){
        perror("The directory doesn't exist");
    }
    struct max_return_values max_val;
    int writable_max = 0;
    char * writable_max_name;
    int nonwritable_max = 0;
    char * nonwritable_max_name;
    struct dirent * dir;
    long disk_usage = 0;
    while(dir = readdir(direct)){
        char * file = dir->d_name;
        char strfile[FILENAME_MAX + strlen(file)];
        sprintf(strfile, "%s/%s", dirname, file);
        if (stat(strfile, &stats) < 0) {
            fprintf(stderr, "Error: Cannot stat '%s'.\n", file);
        }
        if(S_ISDIR((&stats)->st_mode)){
            if((file[0] == '.' && file[1] == '\0')){
                disk_usage = disk_usage + (&stats)->st_size;
                continue;
            }else if((file[1] == '.' && file[0] == '.')){
                continue;
            }
            
            char strdir[FILENAME_MAX*5];
            sprintf(strdir, "%s/%s", dirname,file);
            max_val = read_through(strdir);
            disk_usage = disk_usage + max_val.disk_size;
            if(max_val.write >= writable_max){
                writable_max = max_val.write;
                writable_max_name = max_val.write_name;
            }if(max_val.nonwrite >= nonwritable_max){
                nonwritable_max = max_val.nonwrite;
                nonwritable_max_name = max_val.nonwrite_name;
                    
            }
        }else if((&stats)->st_mode & write_perm){
            disk_usage = disk_usage + (&stats)->st_size;
            if((&stats)->st_size >= writable_max){
                writable_max = (&stats)->st_size;
                writable_max_name = file;
            }
        }else {
            disk_usage = disk_usage + (&stats)->st_size;
            if((&stats)->st_size >= nonwritable_max){
                nonwritable_max = (&stats)->st_size;
                nonwritable_max_name = file;
            }
        }

    }
    closedir(direct);
    max_val.write = writable_max;
    max_val.nonwrite = nonwritable_max;
    max_val.write_name = writable_max_name;
    max_val.nonwrite_name = nonwritable_max_name;
    max_val.disk_size = disk_usage;
    return max_val;
}
int main(int argc, char * argv[]) {
    struct max_return_values maxes;
    maxes.write = 0;
    maxes.write_name = "";
    maxes.nonwrite = 0;
    maxes.nonwrite_name = "";
    maxes.disk_size = 0;
    if(argc == 2){
        DIR * ifreal;
        if(!(ifreal = opendir(argv[1]))){
            perror("The directory in the argument doesn't exist");
            closedir(ifreal);
            return 1;
        }
        maxes = read_through(argv[1]);
        //puts(maxes.write_name);
    }else{
        perror("Incorrect number of arguments");
        return 1;
    }
    
    printf("Write Max: %d\n", maxes.write);
    printf("Write Max Name: %s\n", maxes.write_name);
    printf("Nonwrite Max: %d\n", maxes.nonwrite);
    printf("Nonwrite Max Name: %s\n", maxes.nonwrite_name);
    printf("Disk Usage of '%s': %ld\n", argv[1], maxes.disk_size);
    
    return 0;
}