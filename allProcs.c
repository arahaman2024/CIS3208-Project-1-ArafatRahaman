/*
* Arafat Rahaman
* MYps Project
* allProcs.c
*/

#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <dirent.h>
#include <ctype.h>

#include "allProcs.h"


typedef struct {                    // created a struct that will store information about processes that were obtained from parsing stat, statm, cmdline
    unsigned int proc_id;
    char proc_state;
    unsigned int proc_utime;
    unsigned int proc_stime;
    int proc_virtual_mem_size;
    char cmdline[4096];
} info_fields;



//             * When no pid is passed as an argument this process ran;
//             * It first goes into the /proc directory, and finds the files in the /proc/ file that are process id's
//             * Once it finds that it is a process it goes into their status file and finds ones that match the MYps.c program user_id
//             * If the process has same user id do the following
//                 * Save the proc file name as proc_id (saved in info struct)
//                 * Get state information, user time, system time from the stat file by parsing it (save them indivdiaully in the struct)
//                 * Go into statm to get virtual memory usage
//                 * Go into the cmdline file to get command line arguements
//                     * done by using a pointer see comments for use information
//                 * Print information (can differ via flags)
            
//             * takes an array that stores flag information. Basically used to let the function know about flags.
//             * return 0 upon completion

int run_all_procs(int *flagArray) {
    FILE *fp;                                           // create a file pointer that will be used throughout this program

    int myPS_UID = getuid();                            // obtain the this program's UID

    DIR *dir;                                       
    struct dirent *files;                               // use dirent to get the files in the /proc file system

    dir = opendir("/proc");                             // open /proc file system
    if (dir == NULL) {
        perror("opendir failed");
        exit(EXIT_FAILURE);
    }

    while ((files = readdir(dir)) != NULL) {            // goes through all files and dirs in /proc
        char *fileName = files->d_name;
        int isNum = 1; // true                          // acts as a boolean, will turn to 0 = false if you find a letter in the name
        for (int i = 0; fileName[i]; i++) {             // only numbered files are process ID's
            if (!isdigit(fileName[i])) {                // if at any point is a letter in the name then not a process ID, so turn boolean false
                isNum = 0;
                break;
            }
        }                                               // if its a number than it can be a process ID

        if (isNum) {                                    // if it is a number, then we can start parseing to see if we can get those with matching UID
            char statusPath[4096] = "";
            strcat(statusPath, "/proc/");
            strcat(statusPath, fileName);
            strcat(statusPath, "/");
            strcat(statusPath, "status");               // goes into /proc/<PID>/status to see if the process's UID matches MYps UID
            
            fp = fopen(statusPath, "r");

            
            char *buffer;
            size_t len = 0;
            ssize_t read;
            int uid;

            while((read = getline(&buffer, &len, fp)) != -1) {
                if (strstr (buffer, "Uid")) {                       // obtains UID
                    sscanf(buffer, "Uid : %d", &uid);

                    if (uid == myPS_UID) {                          // if the file has matching UID, WE ARE IN BUISNESS, need to get all the info to show in MYps
                        
                        info_fields current = {0};                  // create and initialize all values of the struct to 0 for use
                        
                        char statPath[4096] = "";                   // go into /proc/PID=fileName/stat to obtain state, utime, stime
                        strcat(statPath, "/proc/");
                        strcat(statPath, fileName);
                        strcat(statPath, "/");
                        strcat(statPath, "stat");

                        fp = fopen(statPath, "r");
                        char *buffStat;
                        size_t lenStat = 0;
                        ssize_t readStat;

                        // getting the procID and procState
                        readStat = getline(&buffStat, &lenStat, fp);
                        if (readStat == -1) {
                            fprintf(stderr, "Error in reading stat.");
                            return EXIT_FAILURE;
                        }            



                        // getting proc_id
                        current.proc_id = atoi(fileName);
                        // could have done this earlier, realized i could do this instead of parsing the stat line

                        
                        // getting proc_state
                        char *first_paren = strchr(buffStat, ')');                      // read up the the ")"
                        char *findFirstCapLetter = first_paren;
                        while (*findFirstCapLetter) {                                   // while the first capital letter which will be the state
                            if (isupper(*findFirstCapLetter)) {                         // upon research I realized the state will always be a capital letter
                                current.proc_state = *findFirstCapLetter;               // onces found save it to the proc_state field
                                break;
                            }
                            findFirstCapLetter++;
                        }





                        // getting utime & stime                                         // can now start getting utime and stime
                        unsigned int utime_ticks;
                        unsigned int stime_ticks;
                        char lineFromParen[4096] = "";
                        strcpy(lineFromParen, first_paren);                              // start reading from first paren // stored at index 14 and 15 respectively
                                         
                        sscanf(lineFromParen, "%*s %*c %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %d %d", &utime_ticks, &stime_ticks);
            
                        current.proc_utime = utime_ticks;                               // store in the utime and stime, they are both in seconds
                        current.proc_stime = stime_ticks;
                        



                        // going into the statm file here                               // need to go to statm to get the virtual mem size
                        char statmPath[4096] = "";
                        strcat(statmPath, "/proc/");
                        strcat(statmPath, fileName);
                        strcat(statmPath, "/");
                        strcat(statmPath, "statm");

                        fp = fopen(statmPath, "r");

                        char *buffStatm;
                        size_t lenStatm = 0;
                        ssize_t readStatm;


                        readStatm = getline(&buffStatm, &lenStatm, fp);
                        if (readStatm == -1) {
                            fprintf(stderr, "Error in reading statm.");
                            return EXIT_FAILURE;
                        }
                        // handle error
                        // statm is stored as one line, and the first number is the amount of virt mem size
                        char virt_mem_size[4096];
                        int i = 0;
                        while (buffStatm[i] != ' ') {               // keep reading until u find a space, save each number to a char array
                            virt_mem_size[i] = buffStatm[i];
                            i++;
                        }
                        virt_mem_size[i] = '\0';                    // add the eof char
                        
                        current.proc_virtual_mem_size = atoi(virt_mem_size);                    // turn it into a number







                        // cmd_line_parser
                        char cmdlinePath[4096] = "";                                        // need to parse the cmdline
                        strcat(cmdlinePath, "/proc/");
                        strcat(cmdlinePath, fileName);
                        strcat(cmdlinePath, "/");
                        strcat(cmdlinePath, "cmdline");


                        // printf("cmdlinePath: %s\n", cmdlinePath);
                        
                        fp = fopen(cmdlinePath, "r");

                        char *buffcmdLine;
                        size_t lencmdLine = 0;
                        ssize_t readcmdLine;

                        readcmdLine = getline(&buffcmdLine, &lencmdLine, fp);      // /proc/<PID>/cmdline is one line
                        if (readcmdLine == -1) {
                            fprintf(stderr, "Error in reading cmdline.");
                            return EXIT_FAILURE;
                        }
                        char cmdArgs[4096];
                        int j = 0;
                        // printf("Arguments: ");
                        char *args = buffcmdLine;                           // creates a pointer
                        for (; args < buffcmdLine + readcmdLine; args++) {         // moves pointer to one char at a time
                            if (*args == '\0') {                            // if you find a null character just print a space " "
                                // printf(" ");
                                cmdArgs[j] = ' ';
                                j++;
                            }else{                              // otherwise print a char
                                // printf("%c", *args);
                                cmdArgs[j] = *args;
                                j++;
                            }
                        }
                        cmdArgs[j] = '\0';
                        strcpy(current.cmdline, cmdArgs);
                        // printf("current.cmdline: %s\n", current.cmdline);







                        printf("\n");
                        
                        //print pid ALWAYS
                        printf("%d: ", current.proc_id);

                        //print u_time of -U is NOT passed 
                        if (flagArray[1] != 1) {
                            printf("utime = %d, ", current.proc_utime);
                        }

                        // print state info IF -s is passed!
                        if (flagArray[0] == 1) {
                            printf("state = %c, ", current.proc_state);
                        }


                        // print s_time if -S IS passed
                        if (flagArray[2] == 1) {
                            printf("stime = %d, ", current.proc_stime);
                        }

                        // print virt mem consumption IF -v is passed!
                        if (flagArray[3] == 1) {
                            printf("virtual_mem_ussage = %d, ", current.proc_virtual_mem_size);
                        }

                        //print cmdline args as long as -c is NOT passed
                        if (flagArray[4] != 1) {
                            printf("cmdline = [%s] ", current.cmdline);
                        }
                        
                        
                        puts("\n");

                    }   // if condition that says if UID matches the processes' UID get that process
                }
                
            }
            
        }
        
    }

    closedir(dir);
    fclose(fp);
    
    return 0;
}



// * When a pid is passed, it following the -p flag the main function in MYps will recogize that this a pid is used and thus should use this function.
// * First does error checking. If the pid makes it this point it means that it was made sure that it was some characters that could be read as a number, but we can make sure here that it is an actual proc_id. Done via checking if the directory /proc/<pid> opens, if it doesn then fails.
// * If it passes error checking does the following
//     * * Save the proc file name as proc_id (saved in info struct)
//     * Get state information, user time, system time from the stat file by parsing it (save them indivdiaully in the struct)
//     * Go into statm to get virtual memory usage
//     * Go into the cmdline file to get command line arguements
//         * done by using a pointer see comments for use information
//     * Print information (can differ via flags)
// * parameter - takes an array that stores flag information & PID. Basically used to let the function know about flags.
// * return 0 upon completion

int pid_proc(int *infoArray) {
    FILE *fp;

    char procName[1024];
    sprintf(procName, "%d", infoArray[5]);

    char procfilePath[4096];
    strcat(procfilePath, "/proc/");
    strcat(procfilePath, procName);

    // need to verify there is a directory
    DIR *dir;                                       

    dir = opendir(procfilePath);                             // open /proc file system
    if (dir == NULL) {
        fprintf(stderr, "Proccess does not exit!\n");
        exit(EXIT_FAILURE);
    }

    // confirmed at this point that the proc exits in the proc file

    // start parsing
    info_fields current = {0};

    // get proc_id
    current.proc_id = infoArray[5];

    // parse stat file
    char statPath[4096] = "";
    strcat(statPath, "/proc/");
    strcat(statPath, procName);
    strcat(statPath, "/");
    strcat(statPath, "stat");

    fp = fopen(statPath, "r");
    char *buffStat;
    size_t lenStat = 0;
    ssize_t readStat;

    readStat = getline(&buffStat, &lenStat, fp);
    if (readStat == -1) {
        fprintf(stderr, "Error in reading stat.");
        return EXIT_FAILURE;
    }

    // getting the state
    char *first_paren = strchr(buffStat, ')');                      // read up the the ")"
    char *findFirstCapLetter = first_paren;
    while (*findFirstCapLetter) {                                   // while the first capital letter which will be the state
        if (isupper(*findFirstCapLetter)) {                         // upon research I realized the state will always be a capital letter
            current.proc_state = *findFirstCapLetter;               // onces found save it to the proc_state field
            break;
        }
        findFirstCapLetter++;
    }

    // getting utime & stime
    unsigned int utime_ticks;
    unsigned int stime_ticks;
    char lineFromParen[4096] = "";
    strcpy(lineFromParen, first_paren);                              // start reading from first paren // stored at index 14 and 15 respectively
                        
    sscanf(lineFromParen, "%*s %*c %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %d %d", &utime_ticks, &stime_ticks);

    current.proc_utime = utime_ticks;                               // store in the utime and stime, they are both in seconds
    current.proc_stime = stime_ticks;

    
    
    // going into the statm file here                               // need to go to statm to get the virtual mem size
    char statmPath[4096] = "";
    strcat(statmPath, "/proc/");
    strcat(statmPath, procName);
    strcat(statmPath, "/");
    strcat(statmPath, "statm");
    
    fp = fopen(statmPath, "r");

    char *buffStatm;
    size_t lenStatm = 0;
    ssize_t readStatm;

    readStatm = getline(&buffStatm, &lenStatm, fp);
    if (readStatm == -1) {
        fprintf(stderr, "Error in reading statm.");
        return EXIT_FAILURE;
    }
    
    // statm is stored as one line, and the first number is the amount of virt mem size
    char virt_mem_size[4096];
    int i = 0;
    while (buffStatm[i] != ' ') {               // keep reading until u find a space, save each number to a char array
        virt_mem_size[i] = buffStatm[i];
        i++;
    }
    virt_mem_size[i] = '\0';                    // add the eof char
    
    current.proc_virtual_mem_size = atoi(virt_mem_size);   // turn it into a number



    // cmdline parcer

    char cmdlinePath[4096] = "";                                        // need to parse the cmdline
    strcat(cmdlinePath, "/proc/");
    strcat(cmdlinePath, procName);
    strcat(cmdlinePath, "/");
    strcat(cmdlinePath, "cmdline");

    fp = fopen(cmdlinePath, "r");

    char *buffcmdLine;
    size_t lencmdLine = 0;
    ssize_t readcmdLine;

    readcmdLine = getline(&buffcmdLine, &lencmdLine, fp);      // /proc/<PID>/cmdline is one line
    if (readcmdLine == -1) {
        fprintf(stderr, "Error in reading cmdline.");
        return EXIT_FAILURE;
    }
    char cmdArgs[4096];
    int j = 0;
    // printf("Arguments: ");
    char *args = buffcmdLine;                           // creates a pointer
    for (; args < buffcmdLine + readcmdLine; args++) {         // moves pointer to one char at a time
        if (*args == '\0') {                            // if you find a null character just print a space " "
            // printf(" ");
            cmdArgs[j] = ' ';
            j++;
        }else{                              // otherwise print a char
            // printf("%c", *args);
            cmdArgs[j] = *args;
            j++;
        }
    }
    cmdArgs[j] = '\0';
    strcpy(current.cmdline, cmdArgs);





    // print with flag commands in mind

    printf("\n");         
    //print pid ALWAYS
    printf("%d: ", current.proc_id);

    //print u_time of -U is NOT passed 
    if (infoArray[1] != 1) {
        printf("utime = %d, ", current.proc_utime);
    }

    // print state info IF -s is passed!
    if (infoArray[0] == 1) {
        printf("state = %c, ", current.proc_state);
    }


    // print s_time if -S IS passed
    if (infoArray[2] == 1) {
        printf("stime = %d, ", current.proc_stime);
    }

    // print virt mem consumption IF -v is passed!
    if (infoArray[3] == 1) {
        printf("virtual_mem_ussage = %d, ", current.proc_virtual_mem_size);
    }

    //print cmdline args as long as -c is NOT passed
    if (infoArray[4] != 1) {
        printf("cmdline = [%s] ", current.cmdline);
    }
    
    puts("\n");
    
    closedir(dir);
    fclose(fp);
    
    return 0;
};