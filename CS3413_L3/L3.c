/**
 * @author Adam Marciszewski
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <zconf.h>

sem_t s1;

int material[3];


void * smokerThread (void* arg){
  int item = (int) arg;
  while(1){
    sem_wait(&s1);
    if(material[item] == 0){
      printf("Smoker %d makes and smokes the cigarette by using ", (item + 1));
      if(item == 0){
        printf("Tobacco\n");
      }else if(item == 1){
        printf("Paper\n");
      }else if(item == 2){
        printf("Matches\n");
      }
      sleep(rand()%10);
      sem_post(&s1);
    }
  }
}
void * agentThread(){
  int random1, random2, i;
  while(1){
    for(i = 0; i < 3; i++){
      material[i] = 0;
    }
    random1 = rand()%(3);
    material[random1] = 1;
    if(random1 == 0){
      printf("Agent Places Tobacco and ");
    }else if(random1 == 1){
      printf("Agent Places Paper and ");
    }else if(random1 == 2){
      printf("Agent Places Matches and ");
    }
    random2 = rand()%(3);
    while(random1 == random2){
      random2 = rand()%(3);
    }
    material[random2] = 1;
    if(random2 == 0){
      printf("Tobacco\n");
    }else if(random2 == 1){
      printf("Paper\n");
    }else if(random2 == 2){
      printf("Matches\n");
    }
    sem_post(&s1);
    sem_post(&s1);
    sem_post(&s1);
    sem_wait(&s1);
  }
}

int main(){
  sem_init(&s1, 0, 0);
  // sem_init(&s2, 0, 0);
  pthread_t smokers[3];
  int i;
  for(i = 0; i < 3; i++){
    pthread_create(&smokers[i], NULL, smokerThread, (void*)i);
  }
  pthread_t agent;
  pthread_create(&agent, NULL, agentThread, NULL);

  for(i = 0; i < 3; i++){
    pthread_join(smokers[i], NULL);
  }
  pthread_join(agent, NULL);


}
