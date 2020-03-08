/**
 * @author Adam Marciszewski
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//  shortest seek time first
int sstf(int bufSize, int buffer[bufSize], int curr){
  int i, shortest = 9999, bufLoc = -1;

  // find the shortest path
  for(i = 0; i < bufSize; i++){
    if(buffer[i] >= 0 && abs(buffer[i] - curr) < shortest){
      shortest = abs(buffer[i] - curr);
      bufLoc = i;
    }
  }
  return bufLoc;
}

//  scan from left to right, once at end start at 0
int cScan(int bufSize, int buffer[bufSize], int curr){
  int i, shortest = 9999, bufLoc = -1;

  // find the closest next request in buffer
  for(i = 0; i < bufSize; i++){
    if(buffer[i] >= curr && (buffer[i] - curr) < shortest){
      shortest = buffer[i] - curr;
      bufLoc = i;
    }

    //  if the end of the memory has been reached, restart at 0
    if(i == (bufSize-1) && shortest == 9999){
      i = -1;
      curr = 0;
    }
  }
  return bufLoc;
}

int main(int arg, char* argv[]){
  int i, algorithm, discStart, bufSize;

  //  match up command line arguments
  for(i = 1; i <= 5; i++){
    if(strcmp(argv[i], "-f") == 0){
      algorithm = 0;
    }else if(strcmp(argv[i], "-s") == 0){
      algorithm = 1;
    }else if(strcmp(argv[i], "-c") == 0){
      algorithm = 2;
    }else if(strcmp(argv[i], "-p") == 0){
      discStart = atoi(argv[i+1]);
    }else if(strcmp(argv[i], "-b") == 0){
      bufSize = atoi(argv[i+1]);
    }
  }
  char* fileName = argv[6];
  FILE* diskLayout;


  int buffer[bufSize];
  int request;

  //  fill buffer with requests
  int endCount = 0;
  for(i = 0; i < bufSize; i++){
    if(scanf("%d", &request) == 1){
      endCount++;
      buffer[i] = request;
    }else{
      buffer[i] = -1;
    }
  }

  int movement = 0, loc = 0, end = 0, next;
  int curr = discStart;

  //  going through buffer and taking in requests
  while(end != endCount){
    if(scanf("%d", &request) != 1){
      end++;
      request = -1;
    }

      //  first come first serve
      if(algorithm == 0){
        next = buffer[loc];
        buffer[loc] = request;
        loc = (loc + 1)%bufSize;
      }else if(algorithm == 1){ //  shortest seek time first
        loc = sstf(bufSize, buffer, curr);
        if(loc == -1)break;
        next = buffer[loc];
        buffer[loc] = request;
      }else if(algorithm == 2){ //  c-scan
        loc = cScan(bufSize, buffer, curr);
        if(loc == -1){
          break;
        }
        next = buffer[loc];
        buffer[loc] = request;

        // if next is before current means head position restarted at 0
        if(next < curr){
          movement += 9999 - curr;
          movement += 9999;
          curr = 0;
        }
      }

      // calculating total movement of head
      movement += abs(curr - next);
      // printf("%d *-> %d\n", curr, next);
      curr = next;

      int seqNext = curr;
    while(seqNext >= 0){
      if((diskLayout = fopen(fileName, "r")) == NULL){
        printf("Error! opening file");
        // Program exits if the file pointer returns NULL.
        exit(1);
      }

      char line[200];
      int i = 0;
      while( i <= seqNext){
        i++;
        fgets(line, 200, diskLayout);
      }
      fclose(diskLayout);
      const char s[2] = " ";
      char *token = strtok(line, s);
      token = strtok(NULL, s);
      seqNext = atoi(token);
      // printf("%d -> %d\n", curr, seqNext);
      if(seqNext >= 0){
        if(algorithm == 2 && seqNext < curr){
            movement += 9999 - seqNext;
            movement += 9999;
            curr = 0;
        }
        movement += abs(curr - seqNext);
        curr = seqNext;
      }
    }
  }
  printf("Total Movement = %d\n", movement);
  return 0;
}
