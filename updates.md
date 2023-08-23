
github: https://github.com/CIS-3207-S23/project-1-s23-arafatrahaman1

--------------------------------------------------------------------------------------------------------------

test.c 
- this was how I first started sort of getting the idea of how I was going to go about reading and learning about the /proc file system. I realized, like from my previous projects (tuls and tucp) that the /proc file system behaved very similarly to files in really any current work direcotries. 
- I also did a lot of research just using the terminal, seeing what I could find as I navigated the /proc file system. I learned simply opening up a directory with a process ID, and reading what was inside using the cat [ name ] function. (name = a file in a process directory. For example cat /proc/<pid>/cmdline)
- [ NOTE! ] Not really submitting for the deliverable, it's really just an extra file I started using.

=========================================================================================

[week-2-update]
deleted!

--------------------------------------------------------------------------------------------------------------

options.c
- This is where I implemented getOPs.
- input: 
-   /options -p 11901 -v -c
- output: 
-   You entered -p. PID: <11901>
    You entered -v
    You entered -c

everything works, just 

=========================================================================================

[week-2-update]
works but once I attach it to myPS, it dies, doesnt work

--------------------------------------------------------------------------------------------------------------

myPS.c
- This one was a little more complicated. So, I was so confused on deliverables, so I did my best
to show that I am able to do what ever I need to do. This program is able to do two things

1. take the current program's (myPS.c) process ID (you can modify it to really any process ID) and go to /proc/<PID>/cmdline. For example, if you did ./myPS a b c d, it is able to go there and parse it, and it aknowledges the fact that in the cmdline it stores it as ./myPS\0a\0b\0c\0d\0.

2. I realized at this point that for week 1 needed to be able to parse really any file in the /proc file system, so the second thing the program is able to do is given a pathname such as proc/1/stat it is able to parse the entire file as well.
input: ./myPS /proc/1/status
output: entire file of /proc/1/status line by line.

[ NOTE! ]
If you give it an actual path name such as [./myPS /proc/1/staus] the first and second part will work. If you really just wanted to see if the first works such as [./myPS a b c d] that will work too, but the second part will not and let you know that it could not open <whatever argument after after ./myPS>. My entire point of this program was to show that I am able to parse not only the cmdline for a process, but also any folder.


=========================================================================================

[week-2-update]
* currently holds everything that is need if the user simply ran ./MYps. 
* as it sits in main, it parses stat, statm, and cmdline to get everything it needs to potentially show user
* currently storing getoptions() because my makefile doesnt work D:

--------------------------------------------------------------------------------------------------------------

Warmups:
I literally wrote the program to get the number of CPU processors, and my plan was to do all of the other ones as well but I realized I wouldn't have enough time do all of them so, I will be giving the plan or my approach on how I would do the other ones as well.

1. Write a program to report the number of processors and the amount of cache in each CPU.
- (Already did it!) Getting the numer of processors were straight forward. I went into CPU info, and read line by line. If my line had the word "processor" in it, it meant that we were at the beginning of the descriptions of a new processsor. Every time I came across a line that had the the word processor, I incremented by 1. 
- (Will do for week 2) The algorithm for this is straight foward as well. You itterate through all the lines in /proc/cpuinfo. When you come across a line that has the substring "cache size", you parse that line and get the string after the ":". You keep itterating through until you get the the EOF. If you have 4 CPU's you should have 4 cache sizes. You can formate it nicely well.

2. write a program to report the free memory in the system and the number of memory buffers.
- (Will do for Week 2) The algorithm for this is straight foward as well. You itterate through all the lines in /proc/meminfo. When you come across a line that has the substring "MemFree", you parse that line and get the string after the ":". You have to do this once.
- (Will do for Week 2) The algorithm for this is straight foward as well. You itterate through all the lines in /proc/meminfo. When you come across a line that has the substring "Buffers", you parse that line and get the string after the ":". You have to do this once.

=========================================================================================

[week-2-update]
I was able to complete the warm up exercies relatively easy following my algorithm. It was definitly a very benificial to do since it literall did what it promised to do which was warm us up. It also made me have to use certain functions that I was inevitably going to use throughout the actual project such as opening files, reading from get line, parsing the lines I got that was stored in a buffer.

--------------------------------------------------------------------------------------------------------------

[week-2-update]
scratch.c - just storing miselaneous things here.

--------------------------------------------------------------------------------------------------------------

[week-2-update]
MAKEFILE - 
I couldn't get this to work. I was initially just going to attach options.c to MYps.c but I couldn't even get that to run. I know the function int get_options (int argc, char *argv[]) works (i showed that it worked in the main in options.c, and works if I copy past it at the top of MYps which it is currently), but it doesn't work when I try to use getOpts through file headers and stuff. I wanted to resolve the issue with Ryan on tuesday but he wasn't available due to technical difficulties.
