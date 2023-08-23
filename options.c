/*
* Arafat Rahaman
* MYps Project
* options.c
*/

#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h> 

#include "options.h"

// uses flags to keep track of flags passed
// everything is initialized to 0 first
// each flag is checked for, if it is passsed the value will be changed to 1
struct flags {
    int flag_p;
    int flag_s;
    int flag_U;
    int flag_S;
    int flag_v;
    int flag_c;
    int pid;
};

// * parameter - takes argc and *argv[]
//             * returns a struct that has information about flags
//             * These flags are sent into run_all_procs and pid_proc functions via a use of an array!
//                 * flagsArray[5] used for run_all_procs. Each index corresponded to  a flag existing or not. If flag was passed that index value was changed to 1 from 0. Index 0 = -s, Index 1 = -U, Index 2 = -S, 
//                   Index 3 = -v, Index 4 = -c. Did not need to send in information about -p flag because flagArrays was used to run all the proccesses that had matching user id's to MYps.c.
//                 * infoArray[6]. Each index corresponded to  a flag existing or not, and the last index was the pid. If flag was passed that index 
//                   value was changed to 1 from 0. Index 0 = -s, Index 1 = -U, Index 2 = -S, Index 3 = -v, Index 4 = -c, Index 5 = <pid>. This was used in pid_proc() function.

struct flags get_options (int argc, char *argv[]) {
    int opt;
    struct flags f = {0}; // create an instance of the struct flags, intiaize everything to 0
    
    while((opt = getopt(argc, argv, "sUSvcp:")) != -1) {
        switch(opt) {
            case 'p':
                f.flag_p = 1;
                if (atoi(optarg) <= 0) {                                                                // if you simply for -p and don't put a ID after it, it exit program
                    fprintf(stderr, "You did not enter a pid. Exiting program...\n");
                    exit(EXIT_FAILURE);
                }
                f.pid = atoi(optarg);
                break;

            case 's':                                                                                   // all the following are flags we will be dealing with
                f.flag_s = 1;
                break;

            case 'U':
                f.flag_U = 1;
                break;
            
            case 'S':
                f.flag_S = 1;
                break;

            case 'v':
                f.flag_v = 1;
                break;

            case 'c':
                f.flag_c = 1;
                break;

            case '?':                                                                                   // this line will print the unknown flag if types. For example -z is not option
                fprintf(stderr, "Unknown option: %c\n", optopt);
                exit(EXIT_FAILURE);
                break;

            default:                                                                                    // if you put something unknown, it will also exit the program via this line
                fprintf(stderr, "Error due to unknown option.\n");
                exit(EXIT_FAILURE);
        }
        
    }
    return f;       // returns the flag struct
}
