#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

#define PROBLEM_SIZE 1000000

struct job {
    pthread_t tid;
    size_t offset;
    size_t size;
};

static void *worker(void *arg){
	struct job *job = (struct job *) arg;
	for(int i = 0; i < job->size; i++){
		int *value = malloc(sizeof(int));
		if(value == NULL){
			continue;
		}
		*value = job->offset + i;
		printf("Thread %u add %d\n", job->tid, *value);
		queue_add(value);
	}

	for(int i = 0; i < job->size; i++){
		int *value = queue_remove();
		printf("Thread %u remove %d\n", job->tid, *value);
		free(value);
	}

	return NULL;
}

int main(int argc, char *argv[]) {
	if(argc != 2){
		fprintf(stderr, "Usage: %s <number of threads>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int numThreads = atoi(argv[1]);
	struct job *threads = (struct job *) malloc(sizeof(struct job) * numThreads);
	if(threads == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	for(size_t i = 0; i < numThreads; i++) {
		struct job *job  = threads + i;
		job->offset = (PROBLEM_SIZE / numThreads) * i;
		if(i == numThreads - 1){
			job->size = PROBLEM_SIZE - (PROBLEM_SIZE / numThreads) * (numThreads - 1);
		}else{
			job->size = (PROBLEM_SIZE / numThreads);
		}

		pthread_t tid;
		errno = pthread_create(&tid, NULL, worker, job);
		if(errno != 0) {
			perror("pthread_create");
			free(threads);
			exit(EXIT_FAILURE);
		}

		job->tid = tid;
	}

	for(size_t i = 0; i < numThreads; i++) {
		struct job *job = threads + i;
		if(0 != pthread_join(job->tid, NULL)) {
			fprintf(stderr, "pthread_join() failed\n");
		}
	}

	free(threads);

	exit(EXIT_SUCCESS);
}
