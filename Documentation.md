/*
* Arafat Rahaman
* MYps Project
* DOCUMENTATION
*/

--------------------------------------------------------------------------------------------------------------

github: https://github.com/CIS-3207-S23/project-1-s23-arafatrahaman1

--------------------------------------------------------------------------------------------------------------

MYps.c

[Arguments_Usage]
* Allowed arguments
    * (none)
    * -p <proc_id> (whether there is a proc id argument or not, will print proc id)
    * -s displays state of proccess
    * -U DON'T display user time consumed by process (user time displayed by default)
    * -S Display the amount of system time consumed by process
    * -v Display amount of cirtual memory used by program
    * -c DON'T display command line arguments (on by default)

* Ways to run this program
    * Print all the processes THAT MATCH the user_id of the MYps.c program and their infomation (depending on flags) - you simply call make file, execute file. Reference:
        * make
        * ./MYps [flags_here]
    * Request information about a particular process - must toss in the flag -p followed by <pid>
        * make
        * ./MYps -p <pid> [other_flags_here]

* When will it fail (CAN FAIL MORE WAYS, but just talking about arguments passed)
    * Fail if invalid flags are passed (if it doesn't recognize a flag)
    * Fail if -p flag is passed without a proc_ic or if no proc_ids are passed
    * Other points of failur will be discussed later in the documentation


* How does it work?

    * It uses 3 other functions besides main

        * int run_all_procs(int *flagArray);  - in allProcs.c
            * When no pid is passed as an argument this process ran;
            * It first goes into the /proc directory, and finds the files in the /proc/ file that are process id's
            * Once it finds that it is a process it goes into their status file and finds ones that match the MYps.c program user_id
            * If the process has same user id do the following
                * Save the proc file name as proc_id (saved in info struct)
                * Get state information, user time, system time from the stat file by parsing it (save them indivdiaully in the struct)
                * Go into statm to get virtual memory usage
                * Go into the cmdline file to get command line arguements
                    * done by using a pointer see comments for use information
                * Print information (can differ via flags)
            
            * takes an array that stores flag information. Basically used to let the function know about flags.
            * return 0 upon completion


        * int pid_proc(int *infoArray);       - in allProcs.c
            * When a pid is passed, it following the -p flag the main function in MYps will recogize that this a pid is used and thus should use this function.
            * First does error checking. If the pid makes it this point it means that it was made sure that it was some characters that could be read as a number, but we can make sure here that it is an actual proc_id. Done via checking if the directory /proc/<pid> opens, if it doesn then fails.
            * If it passes error checking does the following
                * * Save the proc file name as proc_id (saved in info struct)
                * Get state information, user time, system time from the stat file by parsing it (save them indivdiaully in the struct)
                * Go into statm to get virtual memory usage
                * Go into the cmdline file to get command line arguements
                    * done by using a pointer see comments for use information
                * Print information (can differ via flags)

            * parameter - takes an array that stores flag information & PID. Basically used to let the function know about flags.
            * return 0 upon completion

        
        * struct flags get_options (int argc, char *argv[]);   - in options.c
            * function is used to get the flags that are passed and due error checking
            * checks argv[] and compares it to the acceptable flags

            * -p  <pid> 
            Display process information only for the process whose number is pid. It does not matter if the 
            specified process is owned by the current user. If this option is not present, then display 
            information for all processes of the current user (and only of the current user). You only need to 
            support a single -p  option. 
            
            * -s 
            Display the single-character state information about the process. This information is found in the stat 
            file in the process's directory, by looking at the third ("state") field. Note that the information that 
            you read from the stat  file is a character string.  
            
            * -U 
            DO NOT Display the amount of user time consumed by this process. This information is found in the 
            stat file in the process's directory, by looking at the "utime" field. If this option is not present, then 
            user time information is displayed.  
            
            * -S 
            Display the amount of system time consumed so far by this process. This information is found in the 
            stat file in the process's directory, by looking at the "stime" field.  
            
            * -v 
            Display the amount of virtual memory currently used (in pages) by this program. This information is 
            found in the statm  file in the process's directory, by looking at the  first ("size") field.  
            
            * -c 
            DO NOT Display the command-line that started this program. This information is found in the  
            cmdline file in the process's directory. Be careful with this one, because this file contains a list of null 
            (zero byte) terminated strings. If this option is not present, then command line information is 
            displayed.  
            
            * parameter - takes argc and *argv[]
            * returns a struct that has information about flags
            * These flags are sent into run_all_procs and pid_proc functions via a use of an array!
                * flagsArray[5] used for run_all_procs. Each index corresponded to  a flag existing or not. If flag was passed that index value was changed to 1 from 0. Index 0 = -s, Index 1 = -U, Index 2 = -S, Index 3 = -v, Index 4 = -c. Did not need to send in information about -p flag because flagArrays was used to run all the proccesses that had matching user id's to MYps.c.
                * infoArray[6]. Each index corresponded to  a flag existing or not, and the last index was the pid. If flag was passed that index value was changed to 1 from 0. Index 0 = -s, Index 1 = -U, Index 2 = -S, Index 3 = -v, Index 4 = -c, Index 5 = <pid>. This was used in pid_proc() function.
        

        * How it's put together - essentially MYps is executed with or without flags, and that is paresed via get_options function. Once that information is captured, we have an if/else statement in main of MYps which determins if -p was passed. If it is then pid_proc() function is called, other wise run_all_procs() is called.

    * Uses two structs (flag struct, and a struct that keeps track of parsed information)
        * flag struct is used to keep track of flags passed
        * information tracking struct is used to track of inforamtion needed for the output

--------------------------------------------------------------------------------------------------------------

options.c

* stores the get_options functions used in MYps's main (functionality described above, and in code via comments)

--------------------------------------------------------------------------------------------------------------

allProcs.c

* stored both pid_proc and run_all_procs functions (functionality described above, and in code via comments)

==========================================================================================================================================================

makefile

* used to compile MYps and all the files with functions used in the MYps program. This was done via header files, and a makefile

==========================================================================================================================================================

Testing

* Arguments Testing - tested different variations of arguements passed
    * If invalid flags are passed
        * ./MYps -g
        * ./MYps -c 1234
        * ./MYps -p -c
        * etc
    * False PID testing was done, captured by the point where the directory /proc/<pid> was tried to be opened
    * Spent most of my time working with flag arguments and capturing errors early on.

* Running Code in Background - ran sleepy.c in the background to make sure that any extra process that I had going on that matched my user ID worked.

* Error Handing - Done in the code. Print statements used to show points of failure if any.

==========================================================================================================================================================

Troubles Along the Way

* I had issues with getOPts, had to debug a lot. Resolved
* Had a hard time parsing the cmdline because spaces are stored as '\0', and there also that end of line character so had to navigate through that. Resolved
* Troubles with header files and trying to connect c programs that were used in MYps main.

==========================================================================================================================================================
Warmups:
I literally wrote the program to get the number of CPU processors, and my plan was to do all of the other ones as well but I realized I wouldn't have enough time do all of them so, I will be giving the plan or my approach on how I would do the other ones as well.

1. Write a program to report the number of processors and the amount of cache in each CPU.
- (Already did it!) Getting the numer of processors were straight forward. I went into CPU info, and read line by line. If my line had the word "processor" in it, it meant that we were at the beginning of the descriptions of a new processsor. Every time I came across a line that had the the word processor, I incremented by 1. 
- (Will do for week 2) The algorithm for this is straight foward as well. You itterate through all the lines in /proc/cpuinfo. When you come across a line that has the substring "cache size", you parse that line and get the string after the ":". You keep itterating through until you get the the EOF. If you have 4 CPU's you should have 4 cache sizes. You can formate it nicely well.

2. write a program to report the free memory in the system and the number of memory buffers.
- (Will do for Week 2) The algorithm for this is straight foward as well. You itterate through all the lines in /proc/meminfo. When you come across a line that has the substring "MemFree", you parse that line and get the string after the ":". You have to do this once.
- (Will do for Week 2) The algorithm for this is straight foward as well. You itterate through all the lines in /proc/meminfo. When you come across a line that has the substring "Buffers", you parse that line and get the string after the ":". You have to do this once.

==========================================================================================================================================================

updates.md

File used to keep a journal of what I was doing a long the way.

==========================================================================================================================================================

scratch.c

* Preminiary work to investigate the /proc file system. Kept in case for future use or reference. Also kept code for use if needed. 