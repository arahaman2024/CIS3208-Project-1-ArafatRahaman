#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>

int main(int argc, char *argv[]) {
        
    pid_t my_pid = getpid();
    char pidStr[20];
    sprintf(pidStr, "%d", my_pid);
    printf("My pid: %s\n", pidStr);         
    
    // figures out this current process's pid
    // which will then be used to got to the process's /proc/<PID>/cmdline
    // and print it, thus showing we were able to parse the cmdline at least

    FILE *fp;
    char path[4096];
    strcat(path, "/proc/");
    strcat(path, pidStr);
    strcat(path, "/");
    strcat(path, "cmdline");            // path becomes /proc/<PID>/cmdline

    printf("Pathname: %s\n", path);

    fp = fopen(path, "r");          // opens /proc/<PID>/cmdline

    if (fp == NULL) {
        fprintf(stderr, "Was not able to open /proc/cpuinfo");
        return 0;
    }

    char *buffer = NULL;
    size_t len = 0;
    ssize_t read;

    read = getline(&buffer, &len, fp);      // /proc/<PID>/cmdline is one line
    printf("Buffer: %s\n", buffer);

    printf("Arguments: ");
    char *args = buffer;                    // creates a pointer
    for (; args < buffer + read; args++) {  // moves pointer to one char at a time
        if (*args == '\0') {                // if you find a null character just print a space " "
            printf(" ");
        }else{                              // otherwise print a char
            printf("%c", *args);
        }
    }
    
    
    
    // PART 2

    printf("\n\n\nNavigating through: %s\n", argv[1]);          // if given the proper /proc file system path name, I am able to parse it
    char path2[4096];                                           // I don't end up using path2 for concatanation, but you could if you wanted to do something like /proc/ -> strcat(path2, getpid()) -> strcat (path2, "/")...
    strcat(path2, argv[1]);                                     
    printf("Path 2: %s\n", path2);
    fp = fopen(path2, "r");                                     // opens the pathname
    if (fp == NULL) {
        fprintf(stderr, "Was not able to open: <%s>\n", argv[1]);
        return EXIT_FAILURE;
    }

    char *buffer2 = NULL;
    size_t len2 = 0;
    ssize_t read2;

    printf("Printing: ");
    while ((read2 = getline(&buffer2, &len2, fp)) != -1) {      // read one line at time until the end.
        // read2 = getline(&buffer2, &len2, fp);
        printf("%s", buffer2);
    }

    return 0;
}


    // int opt;
    
    // while((opt = getopt(argc, argv, "sUSvcp:")) != -1) {                                                // flags s, U, S, v, c don't need an argument after it. -p does so this is the syntax
    //     switch(opt) {
            
    //         case 'p':
    //             if (atoi(optarg) == 0) {                                                                // if you simply for -p and don't put a ID after it, it exit program
    //                 printf("You did not enter a PID after -p argument. Exiting program...\n");
    //                 return EXIT_FAILURE;
    //             }
    //             printf("You entered -p. PID: <%d>\n", atoi(optarg));
    //             break;

    //         case 's':                                                                                   // all the following are flags we will be dealing with
    //             printf("You entered -%c\n", opt);
    //             break;

    //         case 'U':
    //             printf("You entered -%c\n", opt);
    //             break;
            
    //         case 'S':
    //             printf("You entered -%c\n", opt);
    //             break;

    //         case 'v':
    //             printf("You entered -%c\n", opt);
    //             break;

    //         case 'c':
    //             printf("You entered -%c\n", opt);
    //             break;

    //         case '?':                                                                                   // this line will print the unknown flag if types. For example -z is not option
    //             printf("Unknown option: %c\n", optopt);
    //             break;

    //         default:                                                                                    // if you put something unknown, it will also exit the program via this line
    //             printf("Error due to unknown option.\n");
    //             exit(EXIT_FAILURE);
    //     }
    // } 