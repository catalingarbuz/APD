#ifndef THREAD_FUNC_H
#define THREAD_FUNC_H

typedef struct {
    short p; 
	  long id;
    pthread_barrier_t* barrier;
    int object_count;
    int generations_count;
    sack_object *objects;
    int sack_capacity;
    individual *current_generation;
    individual *next_generation;
} thread_arg;

//Main thread function
void *thread_function(void *arg);

//Merge function
void merge(int *source, int start, int mid, int end, int *destination);

//Min function
int min(int a, int b);

#endif