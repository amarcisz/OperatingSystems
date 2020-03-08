/**
 * @author Adam Marciszewski
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#define NAME_SIZE 20
#define MAX_LINE_LENGTH 80

typedef struct node Node;
typedef struct user User;

struct user{
  char name[NAME_SIZE];
  int finish;
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

Node* insertOrdered(Node* head, Node* newPro){
  if(head != NULL){
    if(head->arrival > newPro->arrival){
      newPro->next = head;
      head->prev = newPro;
      return newPro;

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

User* addUser(User* head, User* userCurr){
  if(head != NULL){
    User *temp = head;
    if(strcmp(temp->name, userCurr->name) == 0){
      return head;
    }
    while(temp->next != NULL){
      temp = temp->next;
      if(strcmp(temp->name, userCurr->name) == 0){
        return head;
      }
    }
    userCurr->next = head;
  }
  userCurr->finish = 1;
  return userCurr;
}

void cycles(Node* head, Node* pri[], int index, int work[], int cpuNum){
  int i, j;
  for(i = 0; i < cpuNum; i++){
    if(work[i] <= 0 && head != NULL){
      Node* check = head;
      while(check != NULL){
        if(check->inPro == 0 && check->arrival <= index && (pri[i] == NULL || pri[i]->priority > check->priority)){
          pri[i] = check;
        }
        check = check->next;
      }
      if(pri[i] != NULL){
        pri[i]->inPro = 1;
      }
    }
  }
  for(i = 0; i < cpuNum; i++){
    if(pri[i] != NULL){
      if(pri[i]->duration > 0){
        //printf("CPU%d running %s %c Duration %d\n", i, pri[i]->person->name, pri[i]->process, pri[i]->duration);
        pri[i]->duration = pri[i]->duration - 1;
        if(work[i] <= 0){
          printf("\t%d\t%c\n", index, pri[i]->process);
        }
        work[i] = 1;
        if(pri[i]->duration <= 0){
          pri[i]->person->finish = index + 1;
          if(pri[i]->prev == NULL && pri[i]->next != NULL){
            pri[i] = pri[i]->next;
            pri[i]->prev->next = NULL;
            pri[i]->prev = NULL;
            head = pri[i];
            pri[i] = NULL;
          }else if(pri[i]->next == NULL && pri[i]->prev != NULL){
            pri[i]->prev->next = NULL;
            pri[i] = NULL;
          }else if(pri[i]->next != NULL && pri[i]->prev != NULL){
            pri[i]->prev->next = pri[i]->next;
            pri[i]->next->prev = pri[i]->prev;
            pri[i] = NULL;
          }else if(pri[i]->next == NULL && pri[i]->prev == NULL){
            head = NULL;
          }
          work[i] = 0;
          if(head == NULL){
            printf("\t%d\tCPU %d IDLE\n", ++index, i+1);
          }
        }
      }
    }else if(work[i] == 0){
      printf("\t%d\tCPU %d IDLE\n", index, i+1);
      work[i] = -1;
    }
  }
  if(head != NULL){
    cycles(head, pri, ++index, work, cpuNum);
  }
}

void printSum(User* head){
  printf("\t%s\t%d\n", head->name, head->finish);
  if(head->next != NULL){
    printSum(head->next);
  }
};

void printList(Node* head){
  if(head != NULL){
    printf("%s\t%c\t%d\t%d\t%d\n", head->person->name, head->process, head->arrival, head->duration, head->priority);
  }
  if(head->next != NULL){
    printList(head->next);
  }
}

void nullUsersNext(User* head){
  if(head->next != NULL){
    nullUsersNext(head->next);
  }
  head->next = NULL;
}

User* orderUsers(Node* head){
  User* userHead = head->person;
  while(head->next != NULL){
    head = head->next;
    User* tempUser = userHead;
    if(tempUser != head->person){
      while(tempUser->next != NULL){
        if(tempUser == head->person){
          break;
        }
        tempUser = tempUser->next;
      }
      if(tempUser->next == NULL){
        tempUser->next = head->person;
      }
    }
  }
  return userHead;
}

int main(int arg, char* args[]){
  int cpuNum = atoi(args[1]);
  Node* head = NULL;
  User* userHead = NULL;
  char string[200];
  while(getchar() != (int) '\n');
  int x = 0;
  while(scanf("%d", &x) != EOF){
    User* userCurr = (User*) malloc(sizeof(User));
    scanf("%s\t", userCurr->name);
    userCurr->next = NULL;
    userHead  = addUser(userHead, userCurr);
    User* userTemp =  userHead;
    if(strcmp(userTemp->name, userCurr->name) != 0){
      while(userTemp->next != NULL){
        userTemp = userTemp->next;
        if(strcmp(userTemp->name, userCurr->name) == 0){
          break;
        }
      }
    }
    Node* newPro = (Node*) malloc(sizeof(Node));
    newPro->person = userTemp;
    scanf("%c\t%d\t%d\t%d", &newPro->process, &newPro->arrival, &newPro->duration, &newPro->priority);
    newPro->prev = NULL;
    newPro->next = NULL;
    newPro->inPro = 0;
    head = insertOrdered(head, newPro);
  }
  nullUsersNext(userHead);
  userHead = orderUsers(head);
//  printList(head);
  printf("\nThis would result in:\n\tTime\tJob\n");
  int work[cpuNum], i;
  Node* pri[cpuNum];
  for(i = 0; i < cpuNum; i++){
    work[i] = -1;
    pri[i] = NULL;
  }

  cycles(head, pri, 0, work, cpuNum);

  printf("\n\tSummary:\n");
  printSum(userHead);
  return EXIT_SUCCESS;
}
