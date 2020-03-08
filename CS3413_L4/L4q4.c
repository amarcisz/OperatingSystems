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
  int size = 1024 * 1024;
  int i = 0;
  while(1){
    malloc(size);
    size = size*2;
    if( i == loop/3 || i == loop/2){
      printf("Sleep 3\n");
      sleep(3);
    }
    i++;
    loop += i;
  }
  return 0;
}
/*
Q4:
I had to constantly allocate more and more memory.
The system memory never went all the way down to 0.
*/
