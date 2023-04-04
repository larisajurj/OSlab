#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>

void results_for_file(struct stat file, char name[]){
    char options;
    while((options = getchar()) != '\n'){
        if(options == 'n'){
            printf("The name of the file is:%s\n", name);
        }else if(options == 'h'){
            printf("The number of hard links is: %ld\n", file.st_nlink);
        }else if(options == 'd'){
            printf("The size of the file is :%ld \n", file.st_size);
        }else if(options == 'm'){
            printf("The time of the last modification is: %ld\n", file.st_mtime);
        }else if(options == 'a'){
            printf("The access rights are: \n");
            if(S_IRWXU&file.st_mode)
                printf("owner has read, write, and execute permission\n");
            if(S_IRUSR&file.st_mode)
                printf("owner has read permission\n");
            if(S_IWUSR&file.st_mode)
                printf("owner has write permission\n");
            if(S_IXUSR&file.st_mode)
                printf("owner has execute permission\n");
            if(S_IRWXG&file.st_mode)
                printf("group has read, write, and execute permission\n");
            if(S_IRGRP&file.st_mode)
                printf("group has read permission\n");
            if(S_IWGRP&file.st_mode)
                printf("group has write permission\n");
            if(S_IXGRP&file.st_mode)
                printf("group has execute permission\n");   
            if(S_IRWXO&file.st_mode)
                printf("others (not  in group) have read, write, and execute permission\n"); 
        }else if(options == 'l'){
            printf("In order to create a symbolik link, please give the name as a string argument\n");
            char name[20];
            scanf("%s", name);
            printf("The name is %s\n", name);
        }
    }
}

int main(int args, char* argv[]){
    if(args < 2){
        printf("not enough arguments\n");
        printf("send arguments of type: paths to regular files, directories or symbolic link\n");
        return -1;
    }

    for(int i = 1; i < args; i++){
        struct stat statbuf;
        char option;
        if(lstat(argv[i], &statbuf)==0){
           if(S_ISREG(statbuf.st_mode)==1){
            printf("A) regular file\n-n (file name)\n-d (dim/size)\n-h (nr of hard links)\n-m (time of last modif)\n-a (access rights)\n-l (create sym link, give: link name)\n");
            results_for_file(statbuf, argv[i]);
           }else if(S_ISLNK(statbuf.st_mode)==1){
            printf("B) sym link\n-n (link name)\n-l(delete link)\n-d(size of link)\n-z(size of target)\n-a(access rights for sym link\n)");
            scanf("%c", &option);
            printf("%c\n", option);
           }else if(S_ISDIR(statbuf.st_mode)==1){
            printf("C) directories\n-n (name)\n-d (size)\n-a(access rights)\n-c(total number of .c files)");
            scanf("%c", &option);
            printf("%c\n", option);
           }
        }
    }
}