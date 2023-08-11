#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "papi.h"
#include "bucketSort.h"
#include "radixSort.h"


static int verify_command_line(int argc, char *argv[], int *total_elements, int *num_buckets);

static int alloc_vector(int **m, int n);

static int init_vector(int **m, int n);

static int free_vector(int **m);

static void print_usage(char *msg);

static int my_rand(void);

static int sort_OK(int *m, int n);



#define NUM_EVENTS 4
int Events[NUM_EVENTS] = { PAPI_TOT_CYC, PAPI_TOT_INS, PAPI_L1_DCM, PAPI_L2_DCM };

// PAPI counters' values
long long values[NUM_EVENTS], min_values[NUM_EVENTS];


int main(int argc, char *argv[]) {
	long long start_usec, end_usec, elapsed_usec;
	int total_elements, num_buckets, i;
	int *a;
	int num_hwcntrs = 0;
	
	if(!verify_command_line(argc, argv, &total_elements, &num_buckets)) return 1;

	fprintf(stdout, "Total of %d elements!\n", total_elements);
	fprintf(stdout, "\nSetting up PAPI...");

	// Initialize PAPI 
	PAPI_library_init(PAPI_VER_CURRENT);
	
	// Get the number of hardware counters available
	if((num_hwcntrs = PAPI_num_counters()) <= PAPI_OK)  {
		fprintf(stderr, "PAPI error getting number of available hardware counters!\n");
		return 1;
	}

	fprintf(stdout, "done!\nThis system has %d available counters.\n\n", num_hwcntrs);

	// We will be using at most NUM_EVENTS counters
	if(num_hwcntrs >= NUM_EVENTS) num_hwcntrs = NUM_EVENTS;
	else {
		fprintf(stderr, "Error: there aren't enough counters to monitor %d events!\n", NUM_EVENTS);
		return 1;
	}

	// init vector
	fprintf(stdout, "Initializing Vector...\n");
	if(!init_vector(&a, total_elements)) return 1;

	fprintf(stdout, "done!\n");


	start_usec = PAPI_get_real_usec();


	// Start counting events
	if(PAPI_start_counters(Events, num_hwcntrs) != PAPI_OK) {
		fprintf(stderr, "PAPI error starting counters!\n");
		return 1;
	}


	// Sorting
	bucketSortP(a, total_elements, num_buckets);


	// Stop counting events
	if(PAPI_stop_counters(values, num_hwcntrs) != PAPI_OK) {
		fprintf(stderr, "PAPI error stoping counters!\n");
		return 1;
	}

	end_usec = PAPI_get_real_usec();
	fprintf(stderr, "done!\n");

	elapsed_usec = end_usec - start_usec;

	printf("\nWall clock time: %f secs\n", (float) elapsed_usec / 1000000.0);

	
	// output PAPI counters' values
	for(i = 0 ; i < NUM_EVENTS; i++) {
		char EventCodeStr[PAPI_MAX_STR_LEN];

		if (PAPI_event_code_to_name(Events[i], EventCodeStr) == PAPI_OK)
			fprintf(stdout, "%s = %lld\n", EventCodeStr, values[i]);
		else
			fprintf(stdout, "PAPI UNKNOWN EVENT = %lld\n",values[i]);
	}

	#if NUM_EVENTS > 1

	// evaluate CPI and Texec here
	if((Events[0] == PAPI_TOT_CYC) && (Events[1] == PAPI_TOT_INS)) {
		float CPI = ((float) values[0]) / ((float) values[1]);
		fprintf(stdout, "CPI = %.2f\n", CPI);
	}

	fprintf(stdout, "\n");

	#endif

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