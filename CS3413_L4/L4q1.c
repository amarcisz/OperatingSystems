/**
 * @author Adam Marciszewski
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <unistd.h>

int main(int arg, char* argv[]){
  int loop = atoi(argv[1]);
  int i = 0;
  while(i < loop){
    malloc(1024);
    if( i == loop/3 || i == loop/2){
      printf("Sleep 3\n");
      sleep(3);
    }
    i++;
  }
  return 0;
}

/*
Q1:
while the program is running the program takes up more mememory.
However at the end of the program you have memory that starts being freed.
It begins being freed because the memory locations are no longer in use.
*/
