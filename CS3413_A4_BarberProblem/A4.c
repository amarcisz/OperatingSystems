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

typedef struct customer Customer;
typedef struct barberChair BarberChair;

struct customer{
  int number;
  pthread_t* thread;
  Customer* next;
};

struct barberChair{
  int available;
  pthread_t* thread;
  Customer* currCustomer;
  pthread_mutex_t* lock;
  BarberChair* next;
};

BarberChair* barberHead;
Customer* custHead;
int waitRoom, chairs;
static pthread_mutex_t m1;


void* barberThread(void* arg){
  BarberChair* barber = (BarberChair*) arg;
  pthread_mutex_lock(barber->lock);
  int available = 0, run = 1;
  while(run == 1){
    if(barber->currCustomer != NULL){
      int v1 = rand() %4 + 4;
      // printf("Barber takes %d with haircut for customer %d\n", v1, custInfo->number);
      sleep(v1);
      printf("Customer %d is styling a fresh haircut :)\n", barber->currCustomer->number);

      pthread_mutex_lock(&m1);
      // printf("Barber is Checking Waitroom\n");
      if(custHead != NULL){
        available = 1;
        barber->available = 1;
        barber->currCustomer = custHead;
        custHead = custHead->next;
        waitRoom--;
        //  printf("Getting Customer %d\n", barber->currCustomer->number);
      }else{
        // printf("Barber is going to sleep");
        barber->currCustomer = NULL;
        available = 0;
      }
      pthread_mutex_unlock(&m1);
    }
    if(available == 0){
      barber->available = 0;
      // printf("Sleep\n");
      pthread_mutex_lock(barber->lock);
      available = 1;
    }/*else{
      printf("Not Sleep\n");
    }*/
  }
}

int checkBarbers(Customer* custInfo){
  BarberChair* tempBarber = barberHead;
  while(tempBarber != NULL){
    if(tempBarber->available == 0){
      tempBarber->available = 1;
      tempBarber->currCustomer = custInfo;
      pthread_mutex_unlock(tempBarber->lock);
      //pthread_create(tempBarber->thread, NULL, barberThread, (void*)tempBarber);
      return 1;
    }
    tempBarber = tempBarber->next;
  }
  return 0;
}

void addCustomer(Customer* newCustomer){
  if(custHead == NULL){
    custHead = newCustomer;
  }else{
    Customer* temp = custHead;
    while(temp->next != NULL){
      temp = temp->next;
    }
    temp->next = newCustomer;
  }
  waitRoom++;
  // printf("Customer %d is in a waiting room with %d people\n", newCustomer->number, waitRoom);
}

void* customerThread(void* arg){
  Customer* custInfo = (Customer*) arg;
  pthread_mutex_lock(&m1);
  if(checkBarbers(custInfo) == 0){
    if(waitRoom < chairs){
      addCustomer(custInfo);
      printf("Customer %d is waiting for a haircut.\n", custInfo->number);
    }else{
      printf("Customer %d leaves with a long shaggy mop of hair!\n", custInfo->number);
    }
  }
  pthread_mutex_unlock(&m1);
}

Customer* createCustomer(int customerNum){
  Customer* newCustomer = (Customer*) malloc(sizeof(Customer));
  newCustomer->number = customerNum;
  newCustomer->next = NULL;
  newCustomer->thread = malloc(sizeof(pthread_t));
  pthread_create(newCustomer->thread, NULL, customerThread, (void*) newCustomer);
  return newCustomer;
}

int main(int arg, char* argv[]){
  int arrival;
  if(strcmp(argv[1], "-c")){
    chairs = atoi(argv[2]);
    if(strcmp(argv[3], "-n")){
      arrival = atoi(argv[4]);
    }
  }else if(strcmp(argv[1], "-n")){
    arrival = atoi(argv[2]);
    if(strcmp(argv[3], "-c")){
      chairs = atoi(argv[4]);
    }
  }
  /*printf("Chairs %d\nInterval %d\n", chairs, arrival);
  int i;
  for(i=0; i < arg; i++){
    printf("Arg[%d] = '%d'\n", i, argv[i]);
  }*/
  // initalizing Barber Chairs
  barberHead = (BarberChair*) malloc(sizeof(BarberChair));
  barberHead->available = 0;
  barberHead->next = (BarberChair*) malloc(sizeof(BarberChair));
  barberHead->next->available = 0;
  barberHead->next->next = (BarberChair*) malloc(sizeof(BarberChair));
  barberHead->next->next->available = 0;


  custHead = NULL;
  // printf("Arrival Interval: %d\nNumber of Chairs: %d\n", arrival, chairs);
  barberHead->thread = malloc(sizeof(pthread_t));
  barberHead->lock = malloc(sizeof(pthread_mutex_t));
  pthread_mutex_init(barberHead->lock, NULL);
  barberHead->next->thread = malloc(sizeof(pthread_t));
  barberHead->next->lock = malloc(sizeof(pthread_mutex_t));
  pthread_mutex_init(barberHead->next->lock, NULL);
  barberHead->next->next->thread = malloc(sizeof(pthread_t));
  barberHead->next->next->lock = malloc(sizeof(pthread_mutex_t));
  pthread_mutex_init(barberHead->next->next->lock, NULL);
  pthread_create(barberHead->thread, NULL, barberThread, barberHead);
  pthread_create(barberHead->next->thread, NULL, barberThread, barberHead->next);
  pthread_create(barberHead->next->next->thread, NULL, barberThread, barberHead->next->next);

  pthread_mutex_init(&m1, NULL);
  waitRoom = 0;
  int loop = 1, customerNum = 0, randomNum;
  while(loop = 1){
    randomNum = rand()%(arrival+1);
    sleep(randomNum);
    //printf("Customer %d arrived\n", customerNum);
    createCustomer(customerNum);
    customerNum++;
  }
}
