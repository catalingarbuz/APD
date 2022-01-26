#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

int N;
int P;
int *v;
int *vQSort;
int *vNew;
#define min(a, b) a < b ? a : b
pthread_barrier_t barrier;
pthread_mutex_t mutex;

void merge(int *source, int start, int mid, int end, int *destination) {
	int iA = start;
	int iB = mid;
	int i;

	for (i = start; i < end; i++) {
		if (end == iB || (iA < mid && source[iA] <= source[iB])) {
			destination[i] = source[iA];
			iA++;
		} else {
			destination[i] = source[iB];
			iB++;
		}
	}

	// int i, j, k;
    // int n1 = mid - start + 1;
    // int n2 = end - mid;
  
    // /* create temp arrays */
    // int L[n1], R[n2];
  
    // /* Copy data to temp arrays L[] and R[] */
    // for (i = 0; i < n1; i++)
    //     L[i] = source[start + i];
    // for (j = 0; j < n2; j++)
    //     R[j] = source[mid + 1 + j];
  
    // /* Merge the temp arrays back into arr[l..r]*/
    // i = 0; // Initial index of first subarray
    // j = 0; // Initial index of second subarray
    // k = start; // Initial index of merged subarray
    // while (i < n1 && j < n2) {
    //     if (L[i] <= R[j]) {
    //         source[k] = L[i];
    //         i++;
    //     }
    //     else {
    //         source[k] = R[j];
    //         j++;
    //     }
    //     k++;
    // }
  
    // /* Copy the remaining elements of L[], if there
    // are any */
    // while (i < n1) {
    //     source[k] = L[i];
    //     i++;
    //     k++;
    // }
  
    // /* Copy the remaining elements of R[], if there
    // are any */
    // while (j < n2) {
    //     source[k] = R[j];
    //     j++;
    //     k++;
    // }
}


void mergeSort(int arr[], int l, int r)
{
    if (l < r) {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l + (r - l) / 2;
  
        // Sort first and second halves
        mergeSort(arr, l, m);
		for (int i = 0; i < r; i++) {
			printf("%d ",arr[i]);
		}
		printf("L = %d \n",l);
        mergeSort(arr, m + 1, r);
  
        merge(arr, l, m, r, NULL);
    }
}

void compare_vectors(int *a, int *b) {
	int i;

	for (i = 0; i < N; i++) {
		if (a[i] != b[i]) {
			printf("Sortare incorecta\n");
			return;
		}
	}

	printf("Sortare corecta\n");
}

void display_vector(int *v) {
	int i;
	int display_width = 2 + log10(N);

	for (i = 0; i < N; i++) {
		printf("%*i", display_width, v[i]);
	}

	printf("\n");
}

int cmp(const void *a, const void *b) {
	int A = *(int*)a;
	int B = *(int*)b;
	return A - B;
}

int is_power_of_two(int n)
{
	if (n == 0) {
		return 0;
	}

	return (ceil(log2(n)) == floor(log2(n)));
}

void get_args(int argc, char **argv)
{
	if(argc < 3) {
		printf("Numar insuficient de parametri: ./merge N P (N trebuie sa fie putere a lui 2)\n");
		exit(1);
	}

	N = atoi(argv[1]);
	// if (!is_power_of_two(N)) {
	// 	printf("N trebuie sa fie putere a lui 2\n");
	// 	exit(1);
	// }

	P = atoi(argv[2]);
}

void init()
{
	int i;
	v = malloc(sizeof(int) * N);
	vQSort = malloc(sizeof(int) * N);
	vNew = malloc(sizeof(int) * N);

	if (v == NULL || vQSort == NULL || vNew == NULL) {
		printf("Eroare la malloc!");
		exit(1);
	}

	srand(42);

	for (i = 0; i < N; i++)
		v[i] = rand() % N;
}


void print()
{
	printf("v:\n");
	display_vector(v);
	printf("vQSort:\n");
	display_vector(vQSort);
	compare_vectors(v, vQSort);
}

void *thread_function(void *arg)
{
	int thread_id = *(int *)arg;

	// implementati aici merge sort paralel
	int width, *aux, i;
	for (width = 1; width < N; width = 2 * width) {
		int pieces = N / (2 * width);

		int start = thread_id * pieces / P * 2 * width;
		int end = (thread_id + 1) * pieces / P * 2 * width;
        printf("start = %d end = %d threadID = %d \n", start, end, thread_id);
		if (end > N) {
			end = N;
		}

		for (i = start; i < end; i = i + 2 * width) {
			merge(v, i, i + width, i + 2 * width, vNew);
		}

		pthread_barrier_wait(&barrier);

		if (thread_id == 0) {
		//	printf("width: %d\n", width);
		//	display_vector(v);
		//	display_vector(vNew);
			aux = v;
			v = vNew;
			vNew = aux;
		}

		pthread_barrier_wait(&barrier);
	}

	pthread_exit(NULL);
}
  
int main(int argc, char *argv[])
{
	get_args(argc, argv);
	init();

	int i;
	int thread_id[P];
	pthread_t tid[P];
	
	pthread_barrier_init(&barrier, NULL, P);
	pthread_mutex_init(&mutex, NULL);

	// se sorteaza vectorul etalon
	for (i = 0; i < N; i++)
		vQSort[i] = v[i];
	printf("inainte de sortare\n");
	display_vector(vQSort);
	qsort(vQSort, N, sizeof(int), cmp);

	// se creeaza thread-urile
	for (i = 0; i < P; i++) {
		thread_id[i] = i;
		pthread_create(&tid[i], NULL, thread_function, &thread_id[i]);
	}

	// se asteapta thread-urile
	for (i = 0; i < P; i++) {
		pthread_join(tid[i], NULL);
	}

	// merge sort clasic - trebuie paralelizat
	//mergeSort(v, 0, N - 1);
	// int width, *aux;
	// for (width = 1; width < N; width = 2 * width) {
	// 	for (i = 0; i < N; i = i + 2 * width) {
	// 		merge(v, i, i + width, i + 2 * width, vNew);
	// 	}

	// 	aux = v;
	// 	v = vNew;
	// 	vNew = aux;
	// }
	

	print();

	free(v);
	free(vQSort);
	free(vNew);
	pthread_barrier_destroy(&barrier);
	pthread_mutex_destroy(&mutex);

	return 0;
}
