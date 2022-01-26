#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "genetic_algorithm.h"
#include "thread_func.c"


int main(int argc, char *argv[]) {
    
	// array with all the objects that can be placed in the sack
	sack_object *objects = NULL;

	// number of objects
	int object_count = 0;

	// maximum weight that can be carried in the sack
	int sack_capacity = 0;

	// number of generations
	int generations_count = 0;
    
	// number of Threads 
	int p = 0;

	if (!read_input(&objects, &object_count, &sack_capacity, &generations_count, &p, argc, argv)) {
		return 0;
	}
	
	thread_arg* thread_args = calloc(p, sizeof(thread_arg));
	pthread_t tid[p];

    pthread_barrier_t barrier; 
    int r = pthread_barrier_init(&barrier, NULL, p);
    //Error 
	if (r != 0) {
      fprintf(stderr, "Error at barrier initialization\n");
		return 0;
	}
	
    int i;
	individual *current_generation = (individual*) calloc(object_count, sizeof(individual));
	individual *next_generation = (individual*) calloc(object_count, sizeof(individual));
	// Creating threads and initializing thread's arguments
	for (i = 0; i < p; i++) {
		thread_args[i].p = p;
		thread_args[i].id = i;
		thread_args[i].barrier = &barrier;
		thread_args[i].object_count = object_count;
		thread_args[i].generations_count = generations_count;
		thread_args[i].objects = objects;
		thread_args[i].sack_capacity = sack_capacity;
		thread_args[i].current_generation = current_generation;
		thread_args[i].next_generation = next_generation;
		pthread_create(&tid[i], NULL, run_thread_genetic, &thread_args[i]);
	}

	// se asteapta thread-urile
	for (i = 0; i < p; i++) {
		pthread_join(tid[i], NULL);
	}

	free(objects);
    pthread_barrier_destroy(&barrier);

	return 0;
}
