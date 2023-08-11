#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "quickSort.h"
#include "radixSort.h"
#include "omp.h"

#define initial_size 10

struct buckets {
	int maxBucket;
	size_t used;
  	size_t size;
	int* bucket;
};


void bucketSort(int array[], int n, int num_buckets) {

	struct buckets b[num_buckets];
	int i, j, k;

	int min, max;

	struct timeval start, end;
	double elapsedTime;

	/**
	*	Find min, max and range
	**/

	gettimeofday(&start, NULL);

	min = max = array[0];

	for(i = 1; i < n; i++) {
        if(min > array[i]) min = array[i];
		if(max < array[i]) max = array[i];
    }

	int range = ((max - min) / num_buckets) + 1;

	gettimeofday(&end, NULL);

	elapsedTime = (end.tv_sec - start.tv_sec);
    elapsedTime += (end.tv_usec - start.tv_usec) / 1000000.0;
	printf("\nFind min, max and range: %f secs\n", elapsedTime);

	/**
	*	Create/Fill buckets and reallocate memory
	**/

	gettimeofday(&start, NULL);

	for(i = 0; i < num_buckets; i++) {
		b[i].used = 0;
		b[i].maxBucket = 0;
		b[i].size = initial_size;
		b[i].bucket = (int*) malloc(initial_size * sizeof(int));
	}
	
	for(i = 0; i < n; i++) {

		int index;
		index = (array[i] - min) / range;
		
		if(b[index].used == b[index].size) {
			b[index].size *= 2;
			b[index].bucket = (int*) realloc(b[index].bucket, b[index].size * sizeof(int));
		}

		if(b[index].maxBucket < array[i])
			b[index].maxBucket = array[i];

		b[index].bucket[(b[index].used)++] = array[i];
	}

	gettimeofday(&end, NULL);

	elapsedTime = (end.tv_sec - start.tv_sec);
    elapsedTime += (end.tv_usec - start.tv_usec) / 1000000.0;
	printf("Create/Fill buckets and reallocate memory: %f secs\n", elapsedTime);

	/**
	*	Sort buckets
	**/

	gettimeofday(&start, NULL);

	for(i = 0; i < num_buckets; i++) {
		radixSort(b[i].bucket, b[i].used, b[i].maxBucket);
	}

	gettimeofday(&end, NULL);

	elapsedTime = (end.tv_sec - start.tv_sec);
    elapsedTime += (end.tv_usec - start.tv_usec) / 1000000.0;
	printf("Sort buckets: %f secs\n", elapsedTime);
	
	/**
	*	Put on the original array and free buckets
	**/

	gettimeofday(&start, NULL);

	for(k = 0, i = 0; i < num_buckets; i++) {

		for(j = 0; j < b[i].used; j++) {
			array[k + j] = b[i].bucket[j];
		}

		k += b[i].used;

		free(b[i].bucket);
	}

	gettimeofday(&end, NULL);

	elapsedTime = (end.tv_sec - start.tv_sec);
    elapsedTime += (end.tv_usec - start.tv_usec) / 1000000.0;
	printf("Put on the original array and free buckets: %f secs\n", elapsedTime);
}


void bucketSortP(int array[], int n, int num_buckets) {

	struct buckets b[num_buckets];
	int i, j, k;

	int min, max;

	struct timeval start, end;
	double elapsedTime;

	/**
	*	Find min, max and range
	**/

	gettimeofday(&start, NULL);

	min = max = array[0];

	#pragma omp parallel for reduction(max:max) reduction(min:min) if(n > 1000000)
	for(i = 1; i < n; i++) {
        if(min > array[i]) min = array[i];
		if(max < array[i]) max = array[i];
    }

	int range = ((max - min) / num_buckets) + 1;

	gettimeofday(&end, NULL);

	elapsedTime = (end.tv_sec - start.tv_sec);
    elapsedTime += (end.tv_usec - start.tv_usec) / 1000000.0;
	printf("\nFind min, max and range: %f secs\n", elapsedTime);

	/**
	*	Create/Fill buckets and reallocate memory
	**/

	gettimeofday(&start, NULL);

	for(i = 0; i < num_buckets; i++) {
		b[i].used = 0;
		b[i].maxBucket = 0;
		b[i].size = initial_size;
		b[i].bucket = (int*) malloc(initial_size * sizeof(int));
	}
	
	for(i = 0; i < n; i++) {

		int index;
		index = (array[i] - min) / range;
		
		if(b[index].used == b[index].size) {
			b[index].size *= 2;
			b[index].bucket = (int*) realloc(b[index].bucket, b[index].size * sizeof(int));
		}

		if(b[index].maxBucket < array[i]) 
			b[index].maxBucket = array[i];

		b[index].bucket[(b[index].used)++] = array[i];
	}

	gettimeofday(&end, NULL);

	elapsedTime = (end.tv_sec - start.tv_sec);
    elapsedTime += (end.tv_usec - start.tv_usec) / 1000000.0;
	printf("Create/Fill buckets and reallocate memory: %f secs\n", elapsedTime);

	/**
	*	Sort buckets
	**/

	gettimeofday(&start, NULL);

	if(n > 100000) {
		#pragma omp parallel for
		for(i = 0; i < num_buckets; i++) {
			radixSortP(b[i].bucket, b[i].used, b[i].maxBucket);
		}
	}
	else {
		for(i = 0; i < num_buckets; i++) {
			radixSort(b[i].bucket, b[i].used, b[i].maxBucket);
		}
	}

	gettimeofday(&end, NULL);

	elapsedTime = (end.tv_sec - start.tv_sec);
    elapsedTime += (end.tv_usec - start.tv_usec) / 1000000.0;
	printf("Sort buckets: %f secs\n", elapsedTime);
	
	/**
	*	Put on the original array and free buckets
	**/

	gettimeofday(&start, NULL);

	for(k = 0, i = 0; i < num_buckets; i++) {

		#pragma	omp parallel for if(n > 100000)
		for(j = 0; j < b[i].used; j++) {
			array[k + j] = b[i].bucket[j];
		}

		k += b[i].used;

		free(b[i].bucket);
	}

	gettimeofday(&end, NULL);

	elapsedTime = (end.tv_sec - start.tv_sec);
    elapsedTime += (end.tv_usec - start.tv_usec) / 1000000.0;
	printf("Put on the original array and free buckets: %f secs\n", elapsedTime);
}