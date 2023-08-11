#include <stdio.h>
#include "quickSort.h"


void partition(int *array, int *i, int *j);

void quickSort_internel(int *array, int left, int right);

void quickSort(int* array, int size) {
	
	int i = 0, j = size;
  	
    partition(array, &i, &j);
	
    if(j > 0) {
	    quickSort_internel(array, 0, j);
    }

	if(i < size) {
		quickSort_internel(array, i, size);
	}
}

void quickSortP(int* array, int size) {
	
	int i = 0, j = size;
  	
    partition(array, &i, &j);
	
	#pragma omp parallel
	#pragma omp single nowait
    if(j > 0) {
 		#pragma omp task
	    quickSort_internel(array, 0, j);
    }

	if(i < size) {
        #pragma omp task
		quickSort_internel(array, i, size);
	}
}

void partition(int* array, int *i, int *j) {
	int pivot = array[(*j)];
	
    while ((*i) <= (*j)) {
		
        while (array[(*i)] < pivot)
			(*i)++;
		
        while (array[(*j)] > pivot)
			(*j)--;
		
        if ((*i) <= (*j)) {
			int tmp = array[(*i)];
			array[(*i)] = array[(*j)];
			array[(*j)] = tmp;
			(*i)++;
			(*j)--;
		}
	}
}

void partition_internel(int* array, int *i, int *j) {
	int pivot = array[(*j)];
	
    while((*i) <= (*j)) {
		
        while(array[(*i)] < pivot)
			(*i)++;
		
        while(array[(*j)] > pivot)
			(*j)--;
		
        if((*i) <= (*j)) {
			int tmp = array[(*i)];
			array[(*i)] = array[(*j)];
			array[(*j)] = tmp;
			(*i)++;
			(*j)--;
		}
	}
}

void quickSort_internel(int* array, int left, int right) {
	int i = left, j = right;

    partition_internel(array, &i, &j) ;
	
	if(left < j)
        quickSort_internel(array, left, j);

	if (i < right)
 		quickSort_internel(array, i, right);

}
