#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int main(int argc, char**argv){

  unsigned int tired = 100;
  
  if(argc>1)
    tired = atoi(argv[1]);
  
  printf("Good Night!\n");
  fflush(stdout);
  sleep(tired);
  printf("Good Morning!\n");

}