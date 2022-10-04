#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
// gcc main.c -o main -lpthread &&./main    //to run the program
int x = 0;
pthread_mutex_t mutex;

void *routine()
{
    for (int i = 0; i < 10000000; i++)
    {
        // try to comment mutex_lock and mutex unlock to see the difference

        pthread_mutex_lock(&mutex);
        x++;
        pthread_mutex_unlock(&mutex);
    }
}
int main(int argc, char *argv[])
{

    pthread_t t1, t2, t3, t4;
    // thread_create function take four arguments first one is thread id
    // second one is attriputes thrid is the
    // function will be executed then the fourth is the fudction attriputes
    // trying mutex after race condition
    pthread_mutex_init(&mutex, NULL);

    if (pthread_create(&t1, NULL, &routine, NULL) != 0)
    {
        return 1;
    }
    if (pthread_create(&t2, NULL, &routine, NULL) != 0)
    {
        return 2;
    }
    if (pthread_create(&t3, NULL, &routine, NULL) != 0)
    {
        return 3;
    }
    if (pthread_create(&t4, NULL, &routine, NULL) != 0)
    {
        return 4;
    }
    if (pthread_join(t1, NULL) != 0)
    {
        return 5;
    }
    if (pthread_join(t2, NULL) != 0)
    {
        return 6;
    }
    if (pthread_join(t3, NULL) != 0)
    {
        return 7;
    }
    if (pthread_join(t4, NULL) != 0)
    {
        return 8;
    }
    pthread_mutex_destroy(&mutex);
    printf("%d \n", x);

    return 0;
}
