
#define HAVE_STRUCT_TIMESPEC
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define BARRIER_COUNT 10
int thread_count;
int barrier_thread_counts[BARRIER_COUNT];
pthread_mutex_t barrier_mutex;

void Usage(char *prog_name);
void *Thread_work(void *rank);

int main(int argc, char *argv[])
{
    long thread, i;
    pthread_t *thread_handles;

    if (argc != 2)
        Usage(argv[0]);
    thread_count = strtol(argv[1], NULL, 10);

    thread_handles = (pthread_t *)malloc(thread_count * sizeof(pthread_t));
    for (i = 0; i < BARRIER_COUNT; i++)
        barrier_thread_counts[i] = 0;
    pthread_mutex_init(&barrier_mutex, NULL);

    for (thread = 0; thread < thread_count; thread++)
        pthread_create(&thread_handles[thread], NULL, Thread_work, (void *)thread);

    for (thread = 0; thread < thread_count; thread++)
    {
        pthread_join(thread_handles[thread], NULL);
    }

    pthread_mutex_destroy(&barrier_mutex);
    free(thread_handles);
    return 0;
}

void Usage(char *prog_name)
{

    fprintf(stderr, "usage: %s <number of threads>\n", prog_name);
    exit(0);
}

void *Thread_work(void *rank)
{

    long my_rank = (long)rank;

    for (int i = 0; i < BARRIER_COUNT; i++)
    {
        pthread_mutex_lock(&barrier_mutex);
        barrier_thread_counts[i]++;
        pthread_mutex_unlock(&barrier_mutex);
        while (barrier_thread_counts[i] < thread_count)
            ;
        if (my_rank == 0)
        {
            printf("All threads entered barrier %d\n", i);
            fflush(stdout);
        }
    }

    return NULL;
}
