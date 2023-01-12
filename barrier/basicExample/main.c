#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

pthread_barrier_t barrier;

void *routine(void *args)
{
    sleep(rand() % 10);
    printf("waiting at the barrier...\n");
    sleep(1);
    pthread_barrier_wait(&barrier);
    printf("we passed the barrier...\n");
    sleep(1);
}

int main(int argc, char *argv[])
{
    pthread_t th[10];
    pthread_barrier_init(&barrier, NULL, 10);
    for (int i = 0; i < 10; i++)
    {
        // create pthread contain first pthread_t object second argument attribute
        // third is routine or function that will be executed last is argument
        if (pthread_create(&th[i], NULL, &routine, NULL) != 0)
        {
            perror("failed to create thread");
        }
    }
    for (int i = 0; i < 10; i++)
    {
        if (pthread_join(th[i], NULL) != 0)
        {
            perror("failed to join thread");
        }
    }
    pthread_barrier_destroy(&barrier);
    return 0;
}