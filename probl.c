#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<string.h>
#include<stdlib.h>
#include <unistd.h>
#include <dirent.h>

int isValid2(char test[100], int size){
    int result = 1;
    for(int i = 0; i < size; i++){
        if((test[i] != 'n') && (test[i] != 'l') && (test[i] != 'd') && (test[i] != 't') && (test[i] != 'a')){
            return 0;
        }
        if(test[i] == 'l')
            result = 2;
    }
    return result;
}
int isValid3(char test[100], int size){
    for(int i = 0; i < size; i++){
        if((test[i] != 'n') && (test[i] != 'd') && (test[i] != 'a') && (test[i] != 'c')){
            return 0;
        }
    }
    return 1;
}
void print_access_rights(struct stat file){
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

}
long long calculateDirectorySize(const char *dirPath) {
    long long totalSize = 0;

    DIR *dir = opendir(dirPath);
    if (dir == NULL) {
        perror("Failed to open directory");
        return -1;
    }

    struct dirent *entry;
    struct stat entryInfo;
    char filePath[PATH_MAX];

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;  // Skip the current directory and parent directory
        }

        snprintf(filePath, sizeof(filePath), "%s/%s", dirPath, entry->d_name);

        if (lstat(filePath, &entryInfo) == 0) {
            if (S_ISREG(entryInfo.st_mode)) {
                totalSize += entryInfo.st_size;  // Regular file, add its size
            } else if (S_ISDIR(entryInfo.st_mode)) {
                long long subDirSize = calculateDirectorySize(filePath);  // Recursive call for subdirectory
                if (subDirSize == -1) {
                    closedir(dir);
                    return -1;  // Propagate the error
                }
                totalSize += subDirSize;  // Add the size of the subdirectory
            }
        } else {
            perror("Failed to get file information");
            closedir(dir);
            return -1;
        }
    }

    closedir(dir);
    return totalSize;
}
int results_for_dir(struct stat file, char name[]){
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
        if(!isValid3(full_options, i)){
            printf("\n!!!!  Not a valid option!  !!!!\nPlease select only valid options from the menu:\n\n");
            return 1; // to print the menu again
        }else{
            printf("________________________________\n");
            char options;
            for(int j = 0; j < i; j++){
                options = full_options[j];
                switch (options){
                    case 'n':
                        printf("The name of the directory is:%s\n", name);
                        break;
                    case 'd':   
                        printf("The size of the directory is: %lld\n", calculateDirectorySize(name));
                        break;
                    case 'a':
                        print_access_rights(file);   
                        break;
                    case 'c':
                        //total number of files with c
                        int count = 0;

                        DIR *dir = opendir(name);
                        if (dir == NULL) {
                            perror("Failed to open directory");
                            return -1;
                        }

                        struct dirent *entry;

                        while ((entry = readdir(dir)) != NULL) {
                            if (entry->d_type == DT_REG) {
                                // Check if the file has a ".c" extension
                                const char *extension = strrchr(entry->d_name, '.');
                                if (extension != NULL && strcmp(extension, ".c") == 0) {
                                    count++;
                                }
                            }
                        }

                        closedir(dir);
                        printf("The number of c files in the directory is : %d\n", count);
                        break;
                }
            }
        }
    }
    return 0;
}
int results_for_symlink(struct stat file, char name[]){
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
        int valid = isValid2(full_options, i);
        if(!valid){
            printf("\n!!!!  Not a valid option!  !!!!\nPlease select only valid options from the menu:\n\n");
            return 1; // to print the menu again
        }else{
            printf("________________________________\n");
            char options;
            if(valid == 2){ // the d option
                unlink(name);
                printf("The link was deleted\n");
            }else
                for(int j = 0; j < i; j++){
                    options = full_options[j];
                    switch (options){
                        case 'n':
                            printf("The name of the symbolink link is:%s\n", name);
                            break;
                        case 'l':
                            printf("The size of the symbolic link is :%ld \n", file.st_size);
                            break;
                        case 't':
                            struct stat targetInfo;
                            stat(name, &targetInfo);
                            printf("The size of the target file is: %ld\n", targetInfo.st_size);
                            break;
                        case 'a':
                            print_access_rights(file); 
                            break;
                    }
                }
        }
    }
    return 0;
}

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
                        print_access_rights(file);   
                        break;
                    case 'l':
                        printf("!!!! In order to create a symbolik link, please give the name as a string argument\n");
                        printf("Type in name: ");
                        char name[20];
                        scanf("%s", name);
                        char currentPath[1024];  // Buffer to store the current working directory
                        if (getcwd(currentPath, sizeof(currentPath)) == NULL) {
                            perror("Failed to get current working directory");
                        }
                        const char *linkPath = name;      

                        if (symlink(currentPath, linkPath) == 0) {
                            printf("Symbolic link created successfully.\n");
                        } else {
                            perror("Failed to create symbolic link");
                        }
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
int countLines(char* name){
    FILE *file = fopen(name, "r");
    if (file == NULL) {
        perror("Failed to open file");
    }

    int lineCount = 0;
    char buffer[1024];  // Buffer to store each line

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        lineCount++;
    }
    fclose(file);
    return lineCount;
}
void createTextFile(char* name){
    char *fileName = malloc(strlen(name) + 10);  // Allocate memory for file name
    if (fileName == NULL) {
        perror("Memory allocation error");
        return;
    }
    // Construct the file name with the directory name and .txt extension
    sprintf(fileName, "%s_file.txt", name);

    FILE *file = fopen(fileName, "w");
    if (file == NULL) {
        perror("Failed to create file");
        free(fileName);
        return;
    }
    // File operations (e.g., write content)

    fclose(file);
    free(fileName);
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
                    int print_menu = 1;
                    while(print_menu){
                        printf("B) sym link\n-n (link name)\n-l(delete link)\n-d(size of link)\n-t(size of target)\n-a(access rights for sym link)\n");
                        print_menu = results_for_symlink(statbuf, argv[i]);
                    }
                }else if(S_ISDIR(statbuf.st_mode)==1){
                    int print_menu = 1;
                    while(print_menu){
                        printf("C) directories\n-n (name)\n-d (size)\n-a(access rights)\n-c(total number of .c files)\n");
                        print_menu = results_for_dir(statbuf, argv[i]);
                    }
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
                    if(S_ISREG(statbuf.st_mode)==1 && strstr(argv[i],".c")){
                        if(S_ISREG(statbuf.st_mode)==1){
                            execlp("bash", "bash", "c_errors.sh", argv[i], NULL);
                        }    
                    }else if(S_ISREG(statbuf.st_mode)==1){
                        int lineCount = countLines(argv[i]);
                        printf("The number of lines is: %d\n", lineCount);
                    }else if(S_ISDIR(statbuf.st_mode)==1){
                        createTextFile(argv[i]);
                        printf("%s_file.txt created\n", argv[i]);
                    }else if(S_ISDIR(statbuf.st_mode)==1){
                         if (execlp("chmod", "chmod", "u=rwx,g=rw,o=---", argv[i], NULL) == -1) {
                            perror("Failed to execute chmod command");
                            }
                    }
                    close(pfd[1]);
                    exit(1);
                }
                //the parent reads the output of the string

                close(pfd[1]); /* close the writting end of the pipe */
                FILE* stream = fdopen(pfd[0], "r");
                if(S_ISREG(statbuf.st_mode)==1 && strstr(argv[i],".c")){
                    char errors[10], warnings[10];
                
                    fscanf(stream,"%s",errors);
                    fscanf(stream,"%s",warnings);

                    printf("%s : %.1f\n", argv[i], compute_score(atoi(errors), atoi(warnings)));
                }
                char buffer[1024];  // Buffer to store each line

                while(fgets(buffer, sizeof(buffer), stream) != NULL) {
                    printf("%s", buffer);
                 }
                close(pfd[0]);

            }
        }
        waitpid(p1, &status, 0);
        waitpid(p2, &status, 0);
    }
}