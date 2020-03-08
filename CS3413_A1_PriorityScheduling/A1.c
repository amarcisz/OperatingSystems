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

void cycles(Node* head, Node* pri, int index, int work){

  if(work <= 0){
    pri = head;
    Node* check = pri;
    while(check->next != NULL){
      check = check->next;
      if(pri->priority > check->priority && check->arrival <= index){
        pri = check;
      }
    }
  }

  if(pri->arrival <= index && pri->duration > 0){
    pri->duration = pri->duration - 1;
    if(work <= 0){
      printf("\t%d\t%c\n", index, pri->process);
    }
    work = 1;
    if(pri->duration <= 0){
      pri->person->finish = index + 1;
      if(pri->prev == NULL && pri->next != NULL){
        pri = pri->next;
        pri->prev = NULL;
        head = pri;
      }if(pri->next == NULL && pri->prev != NULL){
        pri->prev->next = NULL;
        pri = head;
      }if(pri->next != NULL && pri->prev != NULL){
        pri->prev->next = pri->next;
        pri->next->prev = pri->prev;
        pri = head;
      }
      work = 0;
    }
    cycles(head, pri, ++index, work);
  }else if(index >= 2 && work <= 0){
    if(work == 0){
      printf("\t%d\tIDLE\n", index);
      work = -1;
    }
    if(pri->next != NULL || pri->prev != NULL){
      work = -1;
      cycles(head, pri, ++index, work);
    }
  }else if((pri->next != NULL || pri->prev != NULL) && work <= 0){
    work = -1;
    cycles(head, pri, ++index, work);
  }
}

void printSum(User* head){
  if(head->next != NULL){
    printSum(head->next);
  }
  printf("\t%s\t%d\n", head->name, head->finish);
};

int main(){
  Node* head = NULL;
  User* userHead = NULL;
  char string[200];
  scanf("%[^\t\n]",string);
  int x = 0;
  while(scanf("%d", &x) != EOF){
    User* userCurr = (User*) malloc(sizeof(User));
    scanf("%s\t", userCurr->name);
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
    head = insertOrdered(head, newPro);
  }

  printf("\nThis would result in:\n\tTime\tJob\n");
  cycles(head, head, 0, 0);
  printf("\n\tSummary:\n");
  printSum(userHead);
  return EXIT_SUCCESS;
}
