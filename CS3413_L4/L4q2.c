/**
 * @author Adam Marciszewski
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include <unistd.h>

int main(int arg, char* argv[]){
  // int loop = atoi(argv[1]);
  int loop = 100000;
  int i = 0;
  int loc1 = 0, loc2 = 0;
  int avg = 0;
  while(i < loop){
    loc1 = malloc(1024);
    if(loc2 != 0){
      if(loc2 > loc1){
        avg += loc2 - loc1;
      }else{
        avg += loc1 - loc2;
      }
    }
    loc2 = loc1;
    if( i == loop/3 || i == loop/2){
      printf("Sleep 3\n");
      sleep(3);
    }
    i++;
  }
  avg = avg/99999;
  printf("Average: %d\n", avg);
  return 0;
}

/*
Q2:
I noticed that the average different between calls to malloc is close to
the same amount of mememory that I allocate. This tells me that the memory gets
allocated in order inside of memory.

*/
