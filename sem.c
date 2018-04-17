/*
	Bounded Buffer Problem
	Semaphore Implementation
	Gregory Wills
	Operating Systems
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

pthread_mutex_t m;
sem_t full, empty, mutex;

int num_consumers=0, num_producers=0, buffer_size=0, num_loops=0;
int numfull = 0, fillptr = 0, useptr = 0;

int got = 0, put=0,prod_sleep=0,cons_sleep=0;

int* buffer;

void do_fill(int value)
{
	buffer[fillptr] = value;
	fillptr = (fillptr+1) % buffer_size;
	numfull++;
}

int do_get()
{
	int tmp = buffer[useptr];
	useptr = (useptr+1) % buffer_size;
	numfull --;
	return tmp;
}

void* producer(void* arg)
{
	while(put < num_loops-prod_sleep)
	{
		prod_sleep++;
		sem_wait(&empty);
		sem_wait(&mutex);
		do_fill(put);
		put++;
		sem_post(&mutex);
		sem_post(&full);
		prod_sleep--;
	}
}

void* consumer(void* arg)
{
	while(got < num_loops-cons_sleep)
	{
		cons_sleep++;
		sem_wait(&full);
		sem_wait(&mutex);
		int tmp = do_get();
		got++;
		printf("%d\n", tmp);
		sem_post(&mutex);
		sem_post(&empty);
		cons_sleep--;
	}
}

int main(int argc, char *argv[])
{
	if(argc < 5){
		printf("Too few arguments\n");
		return 0;
	}
	num_consumers = atoi(argv[1]);
	num_producers = atoi(argv[2]);
	buffer_size = atoi(argv[3]);
	num_loops = atoi(argv[4]);

	buffer = (int *) malloc(buffer_size *sizeof(int));
	pthread_t pid[20], cid[20];
	sem_init(&empty,0,buffer_size);
	sem_init(&full,0,0);
	sem_init(&mutex,0,1);

	// create producer threads
	int g;
	for(g=0; g<num_producers; g++){
		pthread_create(&pid[g], NULL, producer, NULL);
		printf("producer %d created\n",g+1);
	}

	// create consumer threads
	int i;
	for(i=0; i< num_consumers; i++){
		pthread_create(&cid[i], NULL, consumer, NULL);
		printf("consumer %d created\n",i+1);
	}
	// join producer threads
	for(g=0; g < num_producers; g++){
		pthread_join(pid[g], NULL);	
		printf("producer %d joined\n",g+1);
	}
	// join consumer threads
	int j=0;
	for (j; j< num_consumers; j++){
		pthread_join(cid[j],NULL);
		printf("consumer %d joined\n",j+1);
	}
	printf("program done\n");
	return 0;
}
