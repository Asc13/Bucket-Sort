#include <stdio.h>
#include <stdlib.h>
#include "radixSort.h"
#include "omp.h"

#define BITS 8
#define NUM_BUCKETS (1 << BITS)


static int getDigit(int elem, int byte, int mask) {
    return (elem >> byte) & mask;
}


void radixSort(int array[], int n, int max) {
    int *temp = (int*) malloc(n * sizeof(int));
    int mask = NUM_BUCKETS - 1;
    int i, index;

    for(index = 0; (max >> (index * BITS)) > 0; index++) {
        int bucket[NUM_BUCKETS] = {0};
        int byte = (index * BITS);

        for(i = 0; i < n; i++)
            bucket[getDigit(array[i], byte, mask)]++;

        for(i = 1; i < NUM_BUCKETS; i++)
            bucket[i] += bucket[i - 1];

        for(i = n - 1; i >= 0; i--)
            temp[--bucket[getDigit(array[i], byte, mask)]] = array[i];
        
        for(i = 0; i < n; i++)
            array[i] = temp[i];
    }

    free(temp);
}


void radixSortP(int array[], int n, int max) {
    int *temp = (int*) malloc(n * sizeof(int));
    int mask = NUM_BUCKETS - 1;
    int i, index;

    for(index = 0; (max >> (index * BITS)) > 0; index++) {
        int bucket[NUM_BUCKETS] = {0};
        int byte = (index * BITS);

        #pragma omp parallel firstprivate(bucket) 
        {
            #pragma omp for nowait
            for(i = 0; i < n; i++) {
                bucket[getDigit(array[i], byte, mask)]++;
            }

            #pragma omp critical
            for(i = 1; i < NUM_BUCKETS; i++) {
                bucket[i] += bucket[i - 1];
            }

            #pragma omp barrier
            #pragma omp single
            for(i = n - 1; i >= 0; i--) {
                temp[--bucket[getDigit(array[i], byte, mask)]] = array[i];
            }

            #pragma omp for
            for(i = 0; i < n; i++) {
                array[i] = temp[i];
            }
        }
    }

    free(temp);
}