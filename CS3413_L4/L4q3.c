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
    void* temp = malloc(1024);
    printf("%d\n", temp);
    if( i == loop/3 || i == loop/2){
      free(temp);
      printf("Sleep 3\n");
      sleep(3);
    }
    i++;
  }
  return 0;
}

/*
Q3:
Yes the memory that was freed gets reallocated.
*/
