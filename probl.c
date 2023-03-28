#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>

int main(int args, char* argv[]){
    if(args < 2){
        printf("not enough arguments\n");
        printf("send arguments of type: paths to regular files, directories or symbolic link\n");
        return -1;
    }

    for(int i = 1; i < args; i++){
        struct stat statbuf;
        if(lstat(argv[i], &statbuf)==0){
           if(S_ISREG(statbuf.st_mode)==1){
            printf("A) regular file\n-n (file name)\n-d (dim/size)\n-h (nr of hard links)\n-m (time of last modif)\n-a (access rights)\n-l (create sym link, give: link name)\n");
           }
        }
    }
}