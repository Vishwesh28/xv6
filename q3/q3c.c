// num theads are not more than 16
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<pthread.h>
#include <sys/time.h>

#define NUM_CUSTOMERS 10

struct Transaction{
    int from;
    int to;
    int amount;
};

long balance[NUM_CUSTOMERS] ;
long numTrans;
pthread_mutex_t m[NUM_CUSTOMERS];
struct Transaction *allTransactions;
int numThreads;
float sleeptime=0.001;


void* executeTransaction(void*arg){
    // you only need to modify this function.
    int id=*(int*)arg;
    
    for(int i=id;i<numTrans;i+=numThreads){
	sleep(sleeptime);
	int a,b,c;
	a = allTransactions[i].from;
	b = allTransactions[i].to;
	c = allTransactions[i].amount;
	pthread_mutex_lock(&m[a]);
	pthread_mutex_lock(&m[b]);
	balance[a]-=c;
	balance[b]+=c;
	pthread_mutex_unlock(&m[b]);
	pthread_mutex_unlock(&m[a]);
    }
    
}
void printBalance(char message[]);
float timedifference_msec(struct timeval t0, struct timeval t1);

int main(int argc,char*argv[]){

    struct timeval start;
    struct timeval end;
    float elapsed;
    if (argc != 3)
    {
        printf("Usage: ./a.out <infile> <numThreads> \n");
        exit(EXIT_FAILURE);
    }

    // initially all customers have a balance of 1000
    for(int i=0;i<NUM_CUSTOMERS;i++){
        balance[i]=1000;
    }
    printBalance("Balance before transactions:");

    char *fn = argv[1];
    // Read from file
    FILE *fin = fopen(fn, "r");

    fscanf(fin, "%ld\n", &numTrans);
    printf("The number of transactions are : %ld \n", numTrans);

    allTransactions = malloc(sizeof(struct Transaction)*numTrans);
    for(int i=0;i<numTrans;i++){
        fscanf(fin,"%d %d %d",&allTransactions[i].from,&allTransactions[i].to,&allTransactions[i].amount);
    }

    numThreads = atoi(argv[2]);
    pthread_t  p[numThreads];
    int threadID[numThreads];
    for(int i=0;i<NUM_CUSTOMERS;i++){
    	pthread_mutex_init(&m[i],NULL);
    }
    
    gettimeofday(&start, 0);
    for(int i=0;i<numThreads;i++){
        threadID[i]=i;
        if(pthread_create(&p[i],NULL,&executeTransaction,&threadID[i])){
            printf("cannot create thread :%d \n",i);
            return 1;
        }
    }


    for(int i=0;i<numThreads;i++){
        if(pthread_join(p[i],NULL)){
            printf("join error for thread :%d \n",i);
            return 2;
        }
    }
    gettimeofday(&end, 0);
    // Time spent
    elapsed = timedifference_msec(start, end);
    printBalance("Balance after transactions:");
    printf("Time spent: %.0f ms\n", elapsed);

    return 0;
}

void printBalance(char message[]){
    printf("%s\n",message);
    for(int i=0;i<NUM_CUSTOMERS;i++){
        printf("%d:%ld\n",i,balance[i]);
    }
}
float timedifference_msec(struct timeval t0, struct timeval t1)
{
    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}
