/*
* Arafat Rahaman
* MYps Project
* MYps.c
*/

#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <dirent.h>
#include <ctype.h>

#include "options.h"
#include "allProcs.h"

struct flags {      // used to keep track of flags passed
    int flag_p;
    int flag_s;
    int flag_U;
    int flag_S;
    int flag_v;
    int flag_c;
    int pid;
};


int main(int argc, char *argv[]) {

    puts("");
    struct flags f;
    f = get_options(argc, argv);

    if (f.flag_p == 0) {        // if no -p was passed in (thus also no <pid> argument was passed in), we call run_all_procs
        // take the flag struct and store into an array, an extra step but makes it easier
        int flagsArray[5];
        flagsArray[0] = f.flag_s;
        flagsArray[1] = f.flag_U;
        flagsArray[2] = f.flag_S;
        flagsArray[3] = f.flag_v;
        flagsArray[4] = f.flag_c;
        run_all_procs(flagsArray);  // prints info on procs that matches User id of MYps.c
    }else{  // -p was passed
        int infoArray[6];
        infoArray[0] = f.flag_s;
        infoArray[1] = f.flag_U;
        infoArray[2] = f.flag_S;
        infoArray[3] = f.flag_v;
        infoArray[4] = f.flag_c;
        infoArray[5] = f.pid;
        pid_proc(infoArray);        // passes the array to pid_proc
    }

    puts("");

    return 0;
}