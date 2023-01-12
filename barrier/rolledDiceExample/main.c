#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define THREAD_NUM 8

int dice_values[THREAD_NUM];
int status[THREAD_NUM] = {0};

pthread_barrier_t barrierRolledDice;
pthread_barrier_t barrierCalucluated;

void *roll(void *args)
{
    int index = *(int *)args;
    while (1)
    {
        dice_values[index = rand() % 32 + 1];
        pthread_barrier_wait(&barrierRolledDice);
        pthread_barrier_wait(&barrierCalucluated);
        if (status[index] == 1)
        {
            printf("%d rolled %d i won\n", index, dice_values[index]);
        }
        else
        {
            printf("%d rolled %d i lost\n", index, dice_values[index]);
        }
    }
    free(args);
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    pthread_t th[THREAD_NUM];
    int i;
    pthread_barrier_init(&barrierRolledDice, NULL, THREAD_NUM + 1);
    pthread_barrier_init(&barrierCalucluated, NULL, THREAD_NUM + 1);
    for (int i = 0; i < THREAD_NUM; i++)
    {
        int *a = (int *)malloc(sizeof(int));
        *a = i;
        if (pthread_create(&th[i], NULL, &roll, (void *)a) != 0)
        {
            perror("Failed to create thread");
        }
    }

    while (1)
    {
        pthread_barrier_wait(&barrierRolledDice);
        int max = 0;
        for (i = 0; i < THREAD_NUM; i++)
        {
            if (dice_values[i] > max)
            {
                max = dice_values[i];
            }
        }
        for (i = 0; i < THREAD_NUM; i++)
        {
            if (dice_values[i] == max)
            {
                status[i] = 1;
            }
            else
            {
                status[i] = 0;
            }
        }
        sleep(1);
        printf("=== New round starting ===\n");
        pthread_barrier_wait(&barrierCalucluated);
    }
    for (i = 0; i < THREAD_NUM; i++)
    {
        if (pthread_join(th[i], NULL) != 0)
        {
            perror("Failed to join thread");
        }
    }
    pthread_barrier_destroy(&barrierRolledDice);
    pthread_barrier_destroy(&barrierCalucluated);
    return 0;
}