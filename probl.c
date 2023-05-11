#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<string.h>
#include<stdlib.h>
#include <unistd.h>

int isValid1(char test[100], int size){
    for(int i = 0; i < size; i++){
        if((test[i] != 'n') && (test[i] != 'h') && (test[i] != 'd') && (test[i] != 'm') && (test[i] != 'a') && (test[i] != 'l') )
            return 0;
    }
    return 1;
}
int results_for_file(struct stat file, char name[]){
    char input = getchar();
    while(input != '\n' && input != '-'){
        input = getchar();
    }
    if(input == '-'){
        char full_options[100];
        int i = 0;
        while((input = getchar()) != '\n' && i < 100){
            full_options[i++] = input;
        }
        full_options[i] = '\0';
        if(!isValid1(full_options, i)){
            printf("\n!!!!  Not a valid option!  !!!!\nPlease select only valid options from the menu:\n\n");
            return 1; // to print the menu again
        }else{
            printf("________________________________\n");
            char options;
            for(int j = 0; j < i; j++){
                options = full_options[j];
                switch (options){
                    case 'n':
                        printf("The name of the file is:%s\n", name);
                        break;
                    case 'h':
                        printf("The number of hard links is: %ld\n", file.st_nlink);
                        break;
                    case 'd':
                        printf("The size of the file is :%ld \n", file.st_size);
                        break;
                    case 'm':
                        printf("The time of the last modification is: %ld\n", file.st_mtime);
                        break;
                    case 'a':
                        printf("The access rights are: \n");
                        printf("OWNER: \n");
                        printf("Read: ");
                        if(S_IRUSR&file.st_mode)
                            printf("YES\n");
                        else
                            printf("NO\n");
                        printf("Write: ");
                        if(S_IWUSR&file.st_mode)
                            printf("YES\n");
                        else
                            printf("NO\n");

                        printf("Execute: ");
                        if(S_IXUSR&file.st_mode)
                        printf("YES\n");
                        else
                            printf("NO\n");

                        printf("\nGROUP: \n");
                        printf("Read: ");
                        if(S_IRGRP&file.st_mode)
                        printf("YES\n");
                        else
                            printf("NO\n");
                        printf("Write: ");
                        if(S_IWGRP&file.st_mode)
                            printf("YES\n");
                        else
                            printf("NO\n");
                        printf("Execute: ");
                        if(S_IXGRP&file.st_mode)
                            printf("YES\n");
                        else
                        printf("NO\n");  

                        printf("\nOTHERS: \n");
                        printf("Read: ");
                        if(S_IROTH&file.st_mode)
                        printf("YES\n");
                        else
                            printf("NO\n");
                        printf("Write: ");
                        if(S_IWOTH&file.st_mode)
                            printf("YES\n");
                        else
                            printf("NO\n");
                        printf("Execute: ");
                        if(S_IROTH&file.st_mode)
                            printf("YES\n");
                        else
                        printf("NO\n");  
                        break;
                    case 'l':
                        printf("!!!! In order to create a symbolik link, please give the name as a string argument\n");
                        char name[20];
                        scanf("%s", name);
                        printf("The name is %s\n", name);
                        break;

                }
            }
        }
    }
    return 0;
}
float compute_score(int errors, int warnings){
    if(errors & warnings)
        return 10;
    else if( errors != 0)
        return 1;
    else if (warnings > 10 )
        return 2;
    else 
        return 2 + 8 * (10 - warnings)/ 10.0;
}

int main(int args, char* argv[]){
    int status;
    if(args < 2){
        printf("not enough arguments\n");
        printf("send arguments of type: paths to regular files, directories or symbolic link\n");
        return -1;
    }

     for(int i = 1; i < args; i++){
        struct stat statbuf;
        char option;
        pid_t p2, p1;
        //start a process here
        if(lstat(argv[i], &statbuf)==0){
            p1 = fork();
            if(p1 < 0){
                printf("Could not fork\n");
                return -1;}
            else if(p1 == 0){
                if(S_ISREG(statbuf.st_mode)==1){
                    int print_menu = 1;
                    while(print_menu){
                        printf("A) regular file\n-n (file name)\n-d (dim/size)\n-h (nr of hard links)\n-m (time of last modif)\n-a (access rights)\n-l (create sym link, give: link name)\n");
                        print_menu = results_for_file(statbuf, argv[i]);
                    }
                }else if(S_ISLNK(statbuf.st_mode)==1){
                    printf("B) sym link\n-n (link name)\n-l(delete link)\n-d(size of link)\n-z(size of target)\n-a(access rights for sym link\n)");
                    scanf("%c", &option);
                    printf("%c\n", option);
                }else if(S_ISDIR(statbuf.st_mode)==1){
                    printf("C) directories\n-n (name)\n-d (size)\n-a(access rights)\n-c(total number of .c files)");
                    scanf("%c", &option);
                    printf("%c\n", option);
                }
                exit(0);
            }else{
                int pfd[2]; // pfd[0]- read descriptor, pfd[1]-write descriptor
                int newfd;
                if(pipe(pfd)<0){
                    printf("Could not create pipe\n");
                    exit(0);
                }
                //test if it's a regular file
                p2 = fork();
                if(p2 < 0){
                    printf("Could not fork second process\n");
                    return -1;
                }else if(p2 == 0){
                    //the child process reads the output of script
                    close(pfd[0]); //close read descriptor

                    if((newfd =  dup2(pfd[1],1))<0)
                    {
                        perror("Error when calling dup2\n");
                        exit(1);
                    }
                    if(S_ISREG(statbuf.st_mode)==1){
                        execlp("bash", "bash", "c_errors.sh", argv[i], NULL);
                    }
                    close(pfd[1]);
                    exit(1);
                }
                //the parent reads the output of the string
                close(pfd[1]); /* close the writting end of the pipe */
                FILE* stream = fdopen(pfd[0], "r");
                char errors[10], warnings[10];
                
                fscanf(stream,"%s",errors);
                fscanf(stream,"%s",warnings);

                printf("The score is: %.1f\n", compute_score(atoi(errors), atoi(warnings)));
                close(pfd[0]);

            }
        }
        waitpid(p1, &status, 0);
        waitpid(p2, &status, 0);
    }
}