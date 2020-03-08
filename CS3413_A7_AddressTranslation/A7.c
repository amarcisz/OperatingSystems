/**
 * @author Adam Marciszewski
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int arg, char* argv[]){
  unsigned short* pageTable = calloc(256,sizeof(unsigned short));
  int nextFrame = 0;
  int numFrames = atoi(argv[1]);
  char* fileName = argv[2];
  char* ram = calloc(numFrames*256, sizeof(char));

  unsigned short searchAdr;

  // check if frame is valid
  int numFaults = 0;

  while(scanf("%hu", &searchAdr) == 1){
    unsigned short pageAdr = pageTable[searchAdr>>8];
    //printf("Page Adr: %hu\n", pageAdr);
    if((pageAdr&0x0080) == 0){
      numFaults++;
      int i;
      for(i = 0; i < 256; i++){
        if(pageTable[i]&0xff00 == nextFrame*256){
          pageTable[i] = 0;
        }
      }
      FILE* memFile = fopen(fileName, "r");
      char frame[256];

      fseek(memFile, searchAdr&0xff00, SEEK_SET);

      fread(frame, sizeof(char), 256, memFile);
      fclose(memFile);

      for(i = 0; i < 256; i++){
        ram[nextFrame*256+i] = frame[i];
      }
      pageTable[searchAdr>>8] = (nextFrame*256)|0x0080;
      pageAdr = pageTable[searchAdr>>8];
      nextFrame = (nextFrame + 1) % numFrames;
    }

    char value = ram[(pageAdr&0xff00)+(searchAdr&0x00ff)];

    printf("%hu -> %hu -> %c\n", searchAdr, (pageAdr&~0x00ff)+(searchAdr&0x00ff), value);
  }

  printf("Number of Page Faults: %d", numFaults);

}
