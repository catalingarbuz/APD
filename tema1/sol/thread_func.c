#include <pthread.h>
#include "thread_func.h"
#include "genetic_algorithm.h"


void* run_thread_genetic(void *arg)
{
    thread_arg targ = *((thread_arg *) arg);
	int object_count = targ.object_count;
	int generations_count = targ.generations_count;
	sack_object *objects = targ.objects;
	int sack_capacity = targ.sack_capacity;
	individual *current_generation = targ.current_generation;
	individual *next_generation = targ.next_generation;
	individual *tmp = NULL;

    int count, cursor;

	/* set initial generation (composed of object_count individuals with a single item in the sack)
       Parallelized */
    int start = targ.id * object_count / targ.p;
    int end = min((targ.id + 1) * object_count / targ.p, object_count);
	for (int i = start; i < end; ++i) {
		current_generation[i].fitness = 0;
		current_generation[i].chromosomes = (int*) calloc(object_count, sizeof(int));
		current_generation[i].chromosomes[i] = 1;
		current_generation[i].index = i;
		current_generation[i].chromosome_length = object_count;
		current_generation[i].chromosome_count = 1;

		next_generation[i].fitness = 0;
		next_generation[i].chromosomes = (int*) calloc(object_count, sizeof(int));
		next_generation[i].index = i;
		next_generation[i].chromosome_length = object_count;
		next_generation[i].chromosome_count = 0;
	}
	//Wait for all threads to finish their tasks
	pthread_barrier_wait(targ.barrier);
	
	// iterate for each generation
	for (int k = 0; k < generations_count; ++k) {
		cursor = 0;

		// compute fitness and sort by it - Parallelized
        {
			int weight;
	        int profit;
            int count_objects;
			start = targ.id * object_count / targ.p;
 	        end = min((targ.id + 1) * object_count / targ.p, object_count);
	        for (int i = start; i < end; ++i) {
		        weight = 0;
		        profit = 0;
		        count_objects = 0;

		        for (int j = 0; j < current_generation[i].chromosome_length; ++j) {
			        if (current_generation[i].chromosomes[j]) {
				        weight += objects[j].weight;
				        profit += objects[j].profit;
				        count_objects++;
			        }
		        }
                current_generation[i].chromosome_count = count_objects;
		        current_generation[i].fitness = (weight <= sack_capacity) ? profit : 0;
	        }
		}
		// Waiting for all threads to finish previous task
		pthread_barrier_wait(targ.barrier);

        // Give thread 0 task sort by fitness
		if (targ.id == 0) {
		  qsort(current_generation, object_count, sizeof(individual), cmpfunc);
		}
		// All threads are waiting for thread 0 to finish sorting
	   	pthread_barrier_wait(targ.barrier);


		// keep first 30% children (elite children selection) (Parallelized)
		count = object_count * 3 / 10;
		start = targ.id * count / targ.p;
 	    end = min((targ.id + 1) * count / targ.p, count);
		for (int i = start; i < end; ++i) {
			copy_individual(current_generation + i, next_generation + i);
		}
		pthread_barrier_wait(targ.barrier);
		cursor = count;
        

		// mutate first 20% children with the first version of bit string mutation (Parallelized)
		count = object_count * 2 / 10;
		start = targ.id * count / targ.p;
 	    end = min((targ.id + 1) * count / targ.p, count);
		for (int i = start; i < end; ++i) {
			copy_individual(current_generation + i, next_generation + cursor + i);
			mutate_bit_string_1(next_generation + cursor + i, k);
		}
		pthread_barrier_wait(targ.barrier);
		cursor += count;
		

		// mutate next 20% children with the second version of bit string mutation (Parallelized)
		count = object_count * 2 / 10;
		start = targ.id * count / targ.p;
 	    end = min((targ.id + 1) * count / targ.p, count);
		for (int i = start; i < end; ++i) {
			copy_individual(current_generation + i + count, next_generation + cursor + i);
			mutate_bit_string_2(next_generation + cursor + i, k);
		}
		pthread_barrier_wait(targ.barrier);
		cursor += count;
		
     
		// crossover first 30% parents with one-point crossover
		// (if there is an odd number of parents, the last one is kept as such)
		count = object_count * 3 / 10;
		 if (count % 2 == 1) {
		  copy_individual(current_generation + object_count - 1, next_generation + cursor + count - 1);
		  count--;
		 }
		 

	 	start = targ.id * count / targ.p;
		if (start % 2) {
			start++;
		 }
 	    end = min((targ.id + 1) * count / targ.p, count);
		for (int i = start; i < end; i += 2) {
			crossover(current_generation + i, next_generation + cursor + i, k);
		}

	    // Wait for all threads to finish their tasks before switching to new generation
        pthread_barrier_wait(targ.barrier);

		// switch to new generation
		tmp = current_generation;
		current_generation = next_generation;
		next_generation = tmp;

        start = targ.id * object_count / targ.p;
 	    end = min((targ.id + 1) * object_count / targ.p, object_count);
		for (int i = start; i < end; ++i) {
			current_generation[i].index = i;
		}

	 	if (k % 5 == 0 && targ.id == 1) {
		   print_best_fitness(current_generation);
		}

		pthread_barrier_wait(targ.barrier);
	}

    // Wait for all threads to finish their tasks 
    pthread_barrier_wait(targ.barrier);

    if (targ.id == 0) {
	  compute_fitness_function(objects, current_generation, object_count, sack_capacity);
	  qsort(current_generation, object_count, sizeof(individual), cmpfunc);
	  print_best_fitness(current_generation);

	  // free resources for old generation
	  free_generation(current_generation);
	  free_generation(next_generation);

	  // free resources
	  free(current_generation);
	  free(next_generation);
	}

    pthread_exit(NULL);
}

