// Enter the /proc file system
// Navigate to cpuinfo which is stored as a file
// Use fopen, fread, fwrite
#include <stdio.h>
#include <string.h>

int get_cpu_processors () {
    FILE *fp;
    int count = 0;
    fp = fopen("/proc/cpuinfo", "r");           // open /proc/cpuinfo

    if (fp == NULL) {
        fprintf(stderr, "Was not able to open /proc/cpuinfo");
        return 0;
    }

    char *buffer;
    size_t len = 0;
    ssize_t read;

    while((read = getline(&buffer, &len, fp)) != -1) {              // read's one line at a time, until end of the file
        if (strstr(buffer, "processor")) {                          // if the line has the word processor in it, it increments count by 1. 
            count++;
        }
    }

    fclose(fp);
    return count;                                                   // returns count = # of times processor was fine in a line
}

// Write a program to report the amount of cache in each CPU.
int print_cpu_cache_sizes () {
    FILE *fp;
    int count = 0;
    fp = fopen("/proc/cpuinfo", "r");           // open /proc/cpuinfo

    if (fp == NULL) {
        fprintf(stderr, "Was not able to open /proc/cpuinfo");
        return 0;
    }

    char *buffer;
    size_t len = 0;
    ssize_t read;
    int procNum = 0;

    while((read = getline(&buffer, &len, fp)) != -1) {              // read's one line at a time, until end of the file
        if (strstr (buffer, "cache size")) {
            // printf("%s\n", buffer);
            int size;
            char unit[100];
            sscanf(buffer, "cache size : %d %s", &size, unit);
            printf("Size of processor(%d): %d %s\n", procNum, size, unit);

            procNum++;
        }
    }

    fclose(fp);

}

// write a program to report the free memory in the system and the number of memory buffers.

int print_free_mem() {
    FILE *fp;
    int count = 0;
    fp = fopen("/proc/meminfo", "r");           // open /proc/meminfo

    if (fp == NULL) {
        fprintf(stderr, "Was not able to open /proc/meminfo");
        return 0;
    }

    char *buffer;
    size_t len = 0;
    ssize_t read;

    while((read = getline(&buffer, &len, fp)) != -1) {      // while you can read keep reading (multiple lines in this file)
        if (strstr (buffer, "MemFree")) {                   // if memfree is a substring of the line, the get the stuff after MemFree :
            int size;
            char unit[100];
            sscanf(buffer, "MemFree : %d %s", &size, unit);     // store the first a integer (size) and the other as units (str, could be different depending on device)
            printf("Total free memory: %d %s\n", size, unit);
        }
    }
    fclose(fp);

}

int print_mem_buffers () {
    FILE *fp;
    int count = 0;
    fp = fopen("/proc/meminfo", "r");                               // open /proc/meminfo

    if (fp == NULL) {
        fprintf(stderr, "Was not able to open /proc/meminfo");
        return 0;
    }

    char *buffer;
    size_t len = 0;
    ssize_t read;

    while((read = getline(&buffer, &len, fp)) != -1) {          // keep reading until the end
        if (strstr (buffer, "Buffers")) {                       // if you get to a line who has a substring Buffers stop
            int size;
            char unit[100];
            sscanf(buffer, "Buffers : %d %s", &size, unit);     // capture mem buffers, store in the same fashion as explained above
            printf("Memory buffers: %d %s\n", size, unit);      
        }
    }
    fclose(fp);
}


int main(int argc,char **argv)  {
    puts("");                                                       // outputs to screen in clean formating
    printf("Processors: %d\n", get_cpu_processors());
    puts("");
    print_cpu_cache_sizes();
    puts("");
    print_free_mem();
    puts("");
    print_mem_buffers();
    puts("");
    return 0;
}