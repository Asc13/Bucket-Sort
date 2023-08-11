#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include "bucketSort.h"
#include "radixSort.h"


static int verify_command_line(int argc, char *argv[], int *total_elements, int *num_buckets);

static int alloc_vector(int **m, int n);

static int init_vector(int **m, int n);

static int free_vector(int **m);

static void print_usage(char *msg);

static int my_rand(void);

static int sort_OK(int *m, int n);


int main(int argc, char *argv[]) {
	int total_elements, num_buckets;
	int *a;
	
	if(!verify_command_line(argc, argv, &total_elements, &num_buckets)) return 1;

	// init vector
	fprintf(stdout, "Initializing Vector...\n");
	if(!init_vector(&a, total_elements)) return 1;

	fprintf(stdout, "done!\n");

    struct timeval start, end;
	gettimeofday(&start, NULL);
    
	// Sorting
	bucketSortP(a, total_elements, num_buckets);

	gettimeofday(&end, NULL);

	double elapsedTime;
	elapsedTime = (end.tv_sec - start.tv_sec);
    elapsedTime += (end.tv_usec - start.tv_usec) / 1000000.0;
    
	printf("\nWall clock time: %f secs\n", (float) elapsedTime);

	if(sort_OK(a, total_elements)) fprintf(stdout, "Vector is sort...Valid result.");
	else fprintf(stderr, "Sort function not correct.");

	free_vector(&a);

	printf("\nThat's all, folks\n");
	return 0;
}


int verify_command_line(int argc, char *argv[], int *total_elements, int *num_buckets) {
	int val1, val2;

	if(argc != 3) {
		print_usage((char *) "Exactly 2 arguments are required!");
		return 0;
	}

	val1 = atoi(argv[1]);
	val2 = atoi(argv[2]);

	if(val1 <= 0) {
		print_usage((char *) "The vector size is the size of vector and must be a positive integer!");
		return 0;
	}
	else
		*total_elements = val1;

	if(val2 <= 0) {
		print_usage((char *) "The number of buckets must be a positive integer!");
		return 0;
	}
	else
		*num_buckets = val2;
	
	return 1;
}


void print_usage(char *msg) {
	fprintf(stderr, "Command Line Error! %s\n", msg);
}


int sort_OK(int *m, int n) {
	int i;
	
	for(i = 1; i < n; i++)
		if(m[i - 1] > m[i])
			return 0;

	return 1;
}


int alloc_vector(int **m, int n) {

	*m = (int *) malloc(n * sizeof(int));

	if(!(*m)) {
		print_usage((char *) " ");
		return 0;
	}

	return 1;
}


int my_rand(void) {
	return rand() % 10000000;
}


int init_vector(int** m, int n) {
	int i;
	int *ptr;

	if(!alloc_vector(m, n))
		return 0;

	for(i = 0, ptr = (*m); i < n; i++, ptr++)
		*ptr = my_rand();

	return 1;
} 


int free_vector(int** m) {
	free(*m);
	
	*m = NULL;
	
	return 1;
}