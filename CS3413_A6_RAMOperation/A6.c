/**
 * @author Adam Marciszewski
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

typedef struct process Process;

struct process{
  int processID;
  int size;
  Process* next;
  Process* prev;
};
void printProc(Process* headProcess){
  headProcess = headProcess->next;
  while(headProcess != NULL){
    printf("P%d = size %d\n", headProcess->processID, headProcess->size);
    headProcess = headProcess->next;
  }
}

int terminate(Process* headProcess, int termProc){
  Process* temp = headProcess->next;
  while(temp != NULL){
    if(temp->processID == termProc){
      // printf("terminate 1");
      temp->processID = -1;
      if(temp->next != NULL && temp->next->processID == -1){
        temp->next->size += temp->size;
        temp->prev->next = temp->next;
        temp->next->prev = temp->prev;
        temp = temp->next;
      }
      if(temp->prev != NULL && temp->prev->processID == -1){
        temp->prev->size += temp->size;
        temp->prev->next = temp->next;
        if(temp->next != NULL){
          temp->next->prev = temp->prev;
        }
      }
      return 1;
    }
    temp = temp->next;
  }
  return 0;
}

int worstFit(Process* headProcess, Process* newProcess){
  Process* temp = headProcess->next;
  Process* biggest = NULL;
  while(temp != NULL){
    if(temp->processID == -1 && temp->size >= newProcess->size && (biggest == NULL || biggest->size < temp->size)){
      biggest = temp;
    }
    temp = temp->next;
  }
  if(biggest == NULL){
    return 0;
  }else{
    biggest->size = biggest->size - newProcess->size;
    newProcess->next = biggest;
    newProcess->prev = biggest->prev;
    biggest->prev->next = newProcess;
    biggest->prev = newProcess;
    return 1;
  }
}

int bestFit(Process* headProcess, Process* newProcess){
  Process* temp = headProcess->next;
  Process* smallest = NULL;
  while(temp != NULL){
    if(temp->processID == -1 && temp->size >= newProcess->size && (smallest == NULL || smallest->size > temp->size)){
      smallest = temp;
    }
    temp = temp->next;
  }
  if(smallest == NULL){
    return 0;
  }else{
    smallest->size = smallest->size - newProcess->size;
    newProcess->next = smallest;
    newProcess->prev = smallest->prev;
    smallest->prev->next = newProcess;
    smallest->prev = newProcess;
    return 1;
  }
}


int firstFit(Process* headProcess, Process* newProcess){
  // find available memeory
  Process* temp = headProcess->next;
  while(temp != NULL){
    if(temp->processID == -1 && temp->size > newProcess->size){
      temp->size = temp->size - newProcess->size;
      newProcess->next = temp;
      newProcess->prev = temp->prev;
      temp->prev->next = newProcess;
      temp->prev = newProcess;
      return 1;
    }
    temp = temp->next;
  }
  return 0;
}

int main(int arg, char* argv[]){
  int kernel, RAM, processIn, i;
  // identify command line input
  for(i = 1; i <= 5; i++){
    if(strcmp(argv[i], "-k") == 0){
      kernel = atoi(argv[i+1]);
    }
    if(strcmp(argv[i], "-t") == 0){
      RAM = atoi(argv[i+1]);
    }

    //  type of memory allocation
    if(strcmp(argv[i], "-f") == 0){
      processIn = 0;
    }
    if(strcmp(argv[i], "-b") == 0){
      processIn = 1;
    }
    if(strcmp(argv[i], "-w") == 0){
      processIn = 2;
    }
  }

  Process* headProcess = (Process*) malloc(sizeof(Process));
  headProcess->processID = -2;
  headProcess->size = 0;
  headProcess->prev = NULL;

  // create free space (processID = -1 means free space)
  headProcess->next = (Process*) malloc(sizeof(Process));
  headProcess->next->processID = -1;
  headProcess->next->size = (RAM - kernel);
  headProcess->next->next = NULL;
  headProcess->next->prev = headProcess;

  char command[15], sizeTemp[10];
  int size, processID;
  const char c1[2] = "c";
  const char c2[2] = "C";
  const char t1[2] = "t";
  const char t2[2] = "T";

  while(scanf("%s", &command) == 1){

    //create process
    if(command[0] == 'c' || command[0] == 'C'){
      // get the processID
      char *token = strtok(command, c1);
      token = strtok(token, c2);
      processID = atoi(token);
      // printf("ID: %d\n", processID);

      //get the process size
      scanf("%s", &sizeTemp);
      size = atoi(sizeTemp);
      // printf("Size: %d\n", size);

      // create new process
      Process* newProcess = (Process*) malloc(sizeof(Process));
      newProcess->processID = processID;
      newProcess->size = size;

      // Input a process into memeory according to different algorithms
      if(processIn == 0){
        if(firstFit(headProcess, newProcess) == 0){
          // printf("Not Enough Memory\n");
          break;
        }
      }else if(processIn == 1){
        if(bestFit(headProcess, newProcess) == 0){
          // printf("Not Enough Memory\n");
          break;
        }
      }else if(processIn == 2){
        if(worstFit(headProcess, newProcess) == 0){
          // printf("Not Enough Memory\n");
          break;
        }
      }

    // terminate process
    }else if(command[0] == 't' || command[0] == 'T'){
      char *token = strtok(command, t1);
      token = strtok(token, t2);
      int termProc = atoi(token);
      if(terminate(headProcess, termProc) == 0){
        // printf("Process Does Not Exist\n");
        break;
      }
    }
  }

  // Find amount of available memory and allocated memory
  Process* temp = headProcess->next;
  int available = 0, allocated = kernel;
  while(temp != NULL){
    if(temp->processID == -1){
      available += temp->size;
    }else{
      allocated += temp->size;
    }
    temp = temp->next;
  }
  // printProc(headProcess);
  printf("Total available %d Total allocated %d", available, allocated);

}
