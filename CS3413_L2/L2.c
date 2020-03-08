/**
 * @author Adam Marciszewski
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <zconf.h>

typedef struct list List;

struct list{
  int num;
  List* next;
};

int averageOut, minValOut, maxValOut;
int pipe1[2];
int pipe2[2];
int pipe3[2];

static pthread_mutex_t m1;
static pthread_mutex_t m2;
static pthread_mutex_t m3;

void* average(){
  pthread_mutex_lock(&m1);
  int sum = 0, index = 0, number;
  while(1){
    if(read(pipe1[0], &number, sizeof(int)) == 0){
      break;
    }
    sum += number;
    index++;
  }
  averageOut = (int) (sum / index);
  pthread_mutex_unlock(&m1);
}

void* minVal(){
  pthread_mutex_lock(&m2);
  int min, number;
  if(read(pipe2[0], &number, sizeof(int)) != 0){
    min = number;
    while(1){
      if(read(pipe2[0], &number, sizeof(int)) == 0){
        break;
      }
      if(min > number){
        min = number;
      }
    }
    minValOut = min;
  }
  pthread_mutex_unlock(&m2);
}

void* maxVal(){
  pthread_mutex_lock(&m3);
  int max, number;
  if(read(pipe3[0], &number, sizeof(int)) != 0){
    max = number;
    while(1){
      if(read(pipe3[0], &number, sizeof(int)) == 0){
        break;
      }
      if(max < number){
        max = number;
      }
    }
    maxValOut = max;
  }
  pthread_mutex_unlock(&m3);
}

int main(){
  int input;
  List* head = NULL;
  scanf("%d", &input);
  while(input != 0){
    List* newNum = (List*)malloc(sizeof(List));
    newNum->num = input;
    newNum->next = head;
    head = newNum;
    scanf(" %d", &input);
  }

  pipe(pipe1);
  pipe(pipe2);
  pipe(pipe3);

  pthread_mutex_init(&m1, NULL);
  pthread_mutex_init(&m2, NULL);
  pthread_mutex_init(&m3, NULL);

  pthread_t thread1;
  pthread_t thread2;
  pthread_t thread3;
  pthread_create(&thread1, NULL, &minVal, NULL);
  pthread_create(&thread2, NULL, &maxVal, NULL);
  pthread_create(&thread3, NULL, &average, NULL);


  List* newNum = head;
  while(newNum != NULL){
    write(pipe1[1], &newNum->num, sizeof(int));
    write(pipe2[1], &newNum->num, sizeof(int));
    write(pipe3[1], &newNum->num, sizeof(int));
    newNum = newNum->next;
  }
  close(pipe1[1]);
  close(pipe2[1]);
  close(pipe3[1]);


  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  pthread_join(thread3, NULL);

  pthread_mutex_lock(&m1);
  pthread_mutex_lock(&m2);
  pthread_mutex_lock(&m3);
  printf("The average value is %d\nThe minimum value is %d\nThe maximum value is %d\n", averageOut, minValOut, maxValOut);
  pthread_mutex_unlock(&m3);
  pthread_mutex_unlock(&m2);
  pthread_mutex_unlock(&m1);
}
