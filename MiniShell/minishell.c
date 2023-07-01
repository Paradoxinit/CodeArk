/*
Name: Raqeebullah Hasan
Pledge: I pledge my honor that I have abided by the Stevens Honor System.
*/

#include "minishell.h"

volatile sig_atomic_t interrupted = 0;
int rem_dir[CHAR_MAX] = {0}; //levels for __tree
int rem_nonDir[CHAR_MAX] = {0}; //nonDir of levels for tree
int totalDir = 0;
int total_nonDir = 0;

void handler(int signal){
    interrupted = 1;
    return;
}

int __fileCounter(char* direct, char* description){
    DIR* dp = opendir(direct);
    if(dp == NULL) 
    { fprintf(stderr, "Counter: cannot access '%s'. %s.\n", direct, strerror(errno)); exit(EXIT_FAILURE);}
    struct dirent* dp_entry;
    struct stat fileInfo;
    char scratch_Path[PATH_MAX];
    int counter = 0;
    while((dp_entry = readdir(dp)) != NULL){
        if ( strcmp(dp_entry->d_name, ".") == 0 || strcmp(dp_entry->d_name, "..") == 0) {continue;}
        strcpy(scratch_Path, direct); strcat(scratch_Path, "/"); strcat(scratch_Path, dp_entry->d_name);
        stat(scratch_Path, &fileInfo);
        if(strcmp(description, "non-dir") == 0 && !(S_ISDIR(fileInfo.st_mode)) ){
            counter++;
        }else if(strcmp(description, "dir") == 0 && S_ISDIR(fileInfo.st_mode)){
            counter++;
        }
    }
    closedir(dp);
    return counter;
}

size_t __split_str(char* arg, char* result[]){
    //printf("split: %s\n", arg);
    char* token = strtok(arg, " ");
    size_t count = 0;
    while(token != NULL){
        if(result != NULL){
            result[count] = token;
        }
        token = strtok(NULL, " ");
        count++;
    }
    return count;
}

void __ll(char* dirPath){
    printf("Not Implemented!\n");
    return;
}

void __tree(char* dirPath, int depth){
    DIR* dp = opendir(dirPath);
    
    if(dp == NULL) { fprintf(stderr, "tree: cannot access '%s'. %s.\n", dirPath, strerror(errno)); return; }
    
    struct dirent* dp_entry;
    struct stat fileInfo;
    char scratch_Path[PATH_MAX];
    int dir_count = __fileCounter(dirPath, "dir"); 
    int nonDir_count = __fileCounter(dirPath, "non-dir"); //counter for directories and non-directories
    total_nonDir += nonDir_count;
    totalDir += dir_count;
    //printf("D.Count: %d PrevD.Count: %d\n", dir_count, rem_dir[depth-1]);

    while( (dp_entry = readdir(dp)) != NULL ){
        if ( strcmp(dp_entry->d_name, ".") == 0 || strcmp(dp_entry->d_name, "..") == 0) {continue;}
        strcpy(scratch_Path, dirPath); strcat(scratch_Path, "/"); strcat(scratch_Path, dp_entry->d_name);

        if(stat(scratch_Path, &fileInfo) == -1)
        {fprintf(stderr, "stat: Cannot get '%s' info. %s\n", scratch_Path, strerror(errno)); continue;} 

        if (depth > 0) {
            for (int i = 0; i < depth; i++) {
                if(rem_dir[i] > 0 || rem_nonDir[i] > 0)
                { printf("│"); printf("%*s", 3, "");}
                else {printf("%*s", 4, "");}
            }
        }
        if(S_ISDIR(fileInfo.st_mode)){
            dir_count--;
            rem_dir[depth] = dir_count;
            rem_nonDir[depth] = nonDir_count;
            if(rem_dir[depth] == 0) {printf("└── ");}
            else {printf("├── ");}
            printf("%s%s%s\n", GREEN, dp_entry->d_name, DEFAULT);
            if(rem_dir[depth] == 0) { __tree(scratch_Path, depth+1); }
            else { __tree(scratch_Path, depth+1); }
        } else{
            nonDir_count--;
            if(nonDir_count == 0) {printf("└── ");}
            else {printf("├── ");}
            printf("%s\n", dp_entry->d_name); // "" <- place holder
        }

    }
    closedir(dp);
    return;
}

void __ls(char* dirPath){
    DIR* dp = opendir(dirPath);
    if(dp == NULL) {
        fprintf(stderr, "ls: cannot access '%s'. %s.\n", dirPath, strerror(errno));
        return;
    }
    struct dirent* dp_ent;
    struct stat fileInfo;
    char scratch_Path[PATH_MAX];
    while ( (dp_ent = readdir(dp)) != NULL ){
        if ( strcmp(dp_ent->d_name, ".") == 0 || strcmp(dp_ent->d_name, "..") == 0) {continue;}
        strcpy(scratch_Path, dirPath); strcat(scratch_Path, "/"); strcat(scratch_Path, dp_ent->d_name);
        if (lstat(scratch_Path, &fileInfo) == -1){
            fprintf(stderr, "lstat: Failed at '%s'. %s.\n", scratch_Path, strerror(errno));
            continue;
        }
        if(S_ISDIR(fileInfo.st_mode)) {
            printf("%s%s%s\n", GREEN, dp_ent->d_name, DEFAULT);
        }else{
            printf("%s\n", dp_ent->d_name);
        }
    }
    closedir(dp);
    return;
}

void __find(char* direct, char* filename){
    DIR* dp = opendir(direct);
    if(dp == NULL) 
    { fprintf(stderr, "find: Cannot access '%s'. %s.\n", direct, strerror(errno)); return; }
    struct dirent* entry;
    char scratch_Path[PATH_MAX];
    while((entry = readdir(dp)) != NULL){
        struct stat info;
        if ( strcmp((entry->d_name), ".") == 0 || strcmp(entry->d_name, "..") == 0) 
        {continue;}
        strcpy(scratch_Path, direct); strcat(scratch_Path, "/"); strcat(scratch_Path, entry->d_name);
        //puts(scratch_Path);
        if(stat(scratch_Path, &info) == -1)
        {fprintf(stderr, "stat: Cannot get '%s' info. %s\n", scratch_Path, strerror(errno)); continue;}

        if(strcmp(entry->d_name, filename) == 0)
        { printf("%s\n", scratch_Path); continue;}

        else if(S_ISDIR(info.st_mode))
        { __find(scratch_Path, filename);} //Recursion!!!
    }
    closedir(dp);
    return;
}



int main(){

    char scanner[CHAR_MAX]; //token
    char tempSc[CHAR_MAX]; //to evaluate the argc
    struct sigaction action;
    struct passwd *pwd;
    if ((pwd = getpwuid(getuid())) == NULL) 
    { printf("Error: Cannot get passwd entry. %s.\n", strerror(errno)); }
    
    char* homeDir = pwd->pw_dir;
    action.sa_handler = handler;
    if(sigaction(SIGINT, &action, NULL) == -1)
        { fprintf(stderr, "Error: Cannot register signal handler. %s.\n", strerror(errno)); exit(EXIT_FAILURE);}

    while(1){
        if(interrupted)
        { interrupted = 0; puts(""); continue; }

        char* curr_dir = getcwd(NULL, 0);
        if(curr_dir == NULL) { fprintf(stderr, "%s\n", strerror(errno)); }

        printf("%s[%s]%s> ", BLUE, curr_dir, DEFAULT);

        if (fgets(scanner, sizeof(scanner), stdin) == NULL){
            if (interrupted)
            { interrupted = 0; puts(""); continue;}
            else{
            fprintf(stderr, "Error: Failed to read from stdin. %s.\n", strerror(errno)); 
            continue;
            }
        }

        strtok(scanner, "\n"); //remove newline

        strcpy(tempSc, scanner);
        size_t argc = __split_str(tempSc, NULL);
        
        char* argv[argc];
        __split_str(scanner, argv);

        if (strcmp(argv[0], "exit") == 0){
            break;
        } else if (strcmp(argv[0], "cd")==0){
            if (argc == 1 || strcmp(argv[1], "~")==0){
                if(chdir(homeDir)==-1) 
                { fprintf(stderr, "Error: Cannot change directory. %s.\n", strerror(errno)); }
            }else if (argc == 2){
                if(argv[1][0] == '~')
                {chdir(homeDir); argv[1] = argv[1]+2;}
                if(chdir(argv[1])==-1) 
                { fprintf(stderr, "Error: Cannot change directory to '%s'. %s.\n", argv[1], strerror(errno)); }
            }else{
                fprintf(stderr, "cd: Too many arguments.\n");
            }
            continue;
        } else if (strcmp(argv[0], "ls")==0){
            if(argc == 1){ __ls("."); continue;}
            else if(argc == 2 && !(strcmp(argv[1], "-l") == 0 || strcmp(argv[1], "-a") == 0) )
            { __ls(argv[1]); continue;}

        } else if (strcmp(argv[0], "find") == 0){
            if(argc != 3)
            {fprintf(stderr, "find: Too many arguments.\n");}
            else { __find(argv[1], argv[2]);}
            continue;
        }else if(strcmp(argv[0], "tree")==0){
            //puts("Running Tree!");
            total_nonDir = 0; total_nonDir = 0;
            if(argc == 1)
            {puts("."); __tree(".", 0); }
            else if (argc == 2) {puts(argv[1]); __tree(argv[1], 0); }
            else { fprintf(stderr, "tree: Too many arguments.\n");}
            printf("\n%d directories, %d files\n", totalDir, total_nonDir);
            continue;
        }
        /*The execution of the Child*/

        pid_t pid = fork();

        if(pid == -1){
            fprintf(stderr, "Error: fork() failed. %s.\n", strerror(errno));
        }
        if (pid == 0){
            char* newargv[argc+1];
            for (size_t i = 0; i < argc; i++) { newargv[i] = argv[i]; }
            newargv[argc] = NULL;
            if(execvp(argv[0], newargv) == -1){
                fprintf(stderr, "Error: exec() failed. %s.\n", strerror(errno));
                exit(EXIT_FAILURE);
            }
        }else {
            int status;
            if(waitpid(pid, &status, 0) == -1 && interrupted == 0){
                fprintf(stderr, "Error: wait() failed. %s.\n", strerror(errno));
            }
        }
    }

    return 0;
}
