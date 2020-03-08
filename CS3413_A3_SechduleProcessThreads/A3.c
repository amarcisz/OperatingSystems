/**
 * @author Adam Marciszewski
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <pthread.h>
#include <unistd.h>
#define NAME_SIZE 20
#define MAX_LINE_LENGTH 80

typedef struct threadInfo ThreadInfo;
typedef struct user User;
typedef struct node Node;

struct threadInfo{
  int index;
  int work;
  int cpuNum;
  Node* queue;
};

struct user{
  char name[NAME_SIZE];
  int finish;
  int arrival;
  User *next;
};

struct node {
  char process;
  int arrival;
  int duration;
  int priority;
  int inPro;
  User *person;
  Node *prev, *next;
};


static pthread_mutex_t m1;
static pthread_mutex_t m2;

//  print's everything on a list of type node
void printList(Node* head){
  if(head != NULL){
    printf("%s\t%c\t%d\t%d\t%d :: %d\n", head->person->name, head->process, head->arrival, head->duration, head->priority, head->inPro);
    if(head->next != NULL){
      printList(head->next);
    }
  }
}

//  inserts new processes into head according to arrival time
Node* insertOrdered(Node* head, Node* newPro){
  if(head != NULL){
    //  inserts at head of list
    if(head->arrival > newPro->arrival){
      newPro->next = head;
      head->prev = newPro;
      return newPro;

    //  finds location in the list inserts node
    }else{
      Node *temp = head;
      while(temp->next != NULL){
        if(temp->next->arrival < newPro->arrival ){
          temp = temp->next;
        }else{
          break;
        }
      }
      newPro->next = temp->next;
      newPro->prev = temp;
      temp->next = newPro;
      if(newPro->next != NULL){
        newPro->next->prev = newPro;
      }
      return head;
    }
  }
    return newPro;
}

//  Add's user to user list by process arrival times
User* addUser(User* head, User* newUser, Node* newProcess){
  // Find if user already exisits
  if(head != NULL){
    User *temp = head;
    User *loc = NULL;
    while(temp != NULL){
      if(strcmp(temp->name, newUser->name) == 0){
        if(temp->arrival > newUser->arrival){
          temp->arrival = newUser->arrival;
        }
        newUser = temp;
        newProcess->person = newUser;
        return head;
      }
      //  geting location of placement if process does not aleady exisit
      if(temp->arrival <= newUser->arrival){
        loc = temp;
      }
      temp = temp->next;
    }
    //  sets head of queue to incoming process
    if(loc == NULL){
      newUser->next = head;
      return newUser;
    }
    //  inserts new process into the queue
    newUser->next = loc->next;
    loc->next = newUser;
    return head;
  }else{
    return newUser;
  }
}

//  Resets processes that were run in past cycles
void inProReset(Node* head, int priority){
  Node* check = head;
  while(check != NULL){
    if(check->priority == priority  && check->inPro == -1){
      check->inPro = 0;
      // printf("Reset\t%s\t%c\n", check->person->name, check->process);
    }
    check = check->next;
  }
}

// Finds job for CPU that is not in use
Node* findJob(Node* head){
  if(head != NULL){
    int priority = head->priority;
    Node* check = head;
    Node* pri = NULL;
    int count = 0;
    while(check != NULL){
      if(check->priority != priority) break;
      if(check->inPro == 0 && check->duration > 0){
        pri = check;
        pri->inPro = 1;
        return pri;
      }else if(check->inPro == -1 && check->duration > 0){
        count++;
      }
      check = check->next;
    }
    if(count > 0){
      inProReset(head, priority);
      return findJob(head);
    }else if(check != NULL){
      return findJob(check);
    }
  }
  return NULL;
}

// removes all jobs from queue that are finished
Node* removeJob(Node* head){
  Node* tempQ = head;
  while(tempQ != NULL){
    if(tempQ->duration <= 0){
      if(tempQ->prev == NULL && tempQ->next != NULL){
        tempQ = tempQ->next;
        tempQ->prev = NULL;
        head = tempQ;
      }else if(tempQ->next == NULL && tempQ->prev != NULL){
        tempQ->prev->next = NULL;
        break;
      }else if(tempQ->next != NULL && tempQ->prev != NULL){
        tempQ->next->prev = tempQ->prev;
        tempQ->prev->next = tempQ->next;
      }else if(tempQ->next == NULL && tempQ->prev == NULL){
        head = NULL;
        tempQ = NULL;
        break;
      }
    }
    tempQ = tempQ->next;
  }
  return head;
}

// goes through job cycle
int cycles(Node* head, int index, int work, int cpuNum){
  Node* check;
  Node* pri = NULL;
  if (work > 0) work = 0;
  pthread_mutex_lock(&m1);
  pri = findJob(head);
  pthread_mutex_unlock(&m1);
  //printf("CPU %d:\n", cpuNum);
  //printList(head);

  if(pri != NULL){
    if(pri->duration > 0){
      pri->duration = pri->duration - 1;
      if(work <= 0){
        printf("\t%d\t%c\n", index, pri->process);
      }
      work = 1;
      if(pri->duration <= 0){
        pthread_mutex_lock(&m2);
        pri->person->finish = index + 1;
        pthread_mutex_unlock(&m2);
        work = 0;
        pri = NULL;
      }
    }
  }else if(work == 0){
    printf("\t%d\tCPU %d IDLE\n", index, cpuNum);
    work = -1;
  }
  sleep(1.5);
  return work;
}

//  prints the user summery
void printSum(User* head){
  printf("\t%s\t%d\n", head->name, head->finish);
  if(head->next != NULL){
    printSum(head->next);
  }
};

// moves first item in head into queue
Node* moveToQ(Node* head, Node* queue){
  // if nothing in queue add front of head to queue
  if(queue == NULL){
    queue = head;
    queue->next = NULL;
    queue->prev = NULL;

  // if queue is not empty, find placement in queue
  }else{
    Node* qTemp = queue;
    Node* loc = NULL;

    //  gets insert location
    while(qTemp != NULL){
      if(qTemp->priority <= head->priority){
        loc = qTemp;
      }else{
        break;
      }
      qTemp = qTemp->next;
    }

    //  if placement is head of list
    if(loc == NULL){
      qTemp = head;
      qTemp->next = queue;
      qTemp->next->prev = qTemp;
      qTemp->prev = NULL;
      queue = qTemp;

    // if placement is not head of the list
    }else{
      qTemp = head;
      qTemp->next = loc->next;
      qTemp->prev = loc;
      if(qTemp->next != NULL){
        qTemp->next->prev = qTemp;
      }
      loc->next = qTemp;
    }
  }
  return queue;

}

//  sets all work from 1 to -1
void setPastWork(Node* queue){
  if(queue != NULL){
    if(queue->inPro == 1){
      queue->inPro = -1;
    }
    setPastWork(queue->next);
  }
}

//  method for threads
void* run(void* arg){
  ThreadInfo* info = (ThreadInfo*) arg;
  info->work = cycles(info->queue, info->index, info->work, info->cpuNum);
}

int main(int arg, char* args[]){
  int cpuNum = atoi(args[1]);
  Node* head = NULL;
  User* userHead = NULL;
  //ignores first line
  char string[200];
  while(getchar() != (int) '\n');

  // creates processes until end of file
  int x = 0;
  while(scanf("%d", &x) != EOF){
    User* userCurr = (User*) malloc(sizeof(User));
    Node* newPro = (Node*) malloc(sizeof(Node));
    scanf("%s\t%c\t%d\t%d\t%d", userCurr->name, &newPro->process, &newPro->arrival, &newPro->duration, &newPro->priority);
    userCurr->next = NULL;
    userCurr->arrival = newPro->arrival;
    userCurr->finish = 1;
    newPro->person = userCurr;
    newPro->prev = NULL;
    newPro->next = NULL;
    newPro->inPro = 0;
    userHead  = addUser(userHead, userCurr, newPro);
    head = insertOrdered(head, newPro);
  }

  // print begining of result
  printf("\nThis would result in:\n\tTime\tJob\n");

  // initalizing locks
  pthread_mutex_init(&m1, NULL);
  pthread_mutex_init(&m2, NULL);

  // setting up variable and space
  Node* queue = NULL;
  ThreadInfo* info[cpuNum];
  int index = 0, fin = 0, i;
  for(i = 0; i < cpuNum; i++){
    info[i] = (ThreadInfo*) malloc(sizeof(ThreadInfo));
    info[i]->work = -1;
  }

  // running each cycle until 2 times after finished
  while(fin <= 2){
    if(head != NULL){
      // input items into the queue if their arrival time is at index
      while(head->arrival <= index){
        Node* moveH = head;
        head = head->next;
        queue = moveToQ(moveH, queue);
        if(head != NULL) head->prev = NULL;
        else break;
      }
    }

    //  sets info for thread then creates threads
    pthread_t thread[cpuNum];
    for(i = 0; i < cpuNum; i++){
      info[i]->queue = queue;
      info[i]->index = index;
      info[i]->cpuNum = i+1;
      pthread_create(&thread[i], NULL, run, (void*)info[i]);
    }
    for(i = 0; i < cpuNum; i++){
      pthread_join(thread[i], NULL);
    }

    // removing finished jobs and setting all queue inPro 1's to -1
    queue = removeJob(queue);
    setPastWork(queue);

    // if end of both lists increase finish time
    if(head == NULL && queue == NULL){
      fin++;
    }
    index++;
  }

  printf("\n\tSummary:\n");
  printSum(userHead);
  return EXIT_SUCCESS;
}
