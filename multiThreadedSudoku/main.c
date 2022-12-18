#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int num_threads = 27;

int valid = 0;
pthread_mutex_t mutex;

void isValid()
{
    pthread_mutex_lock(&mutex);
    valid++;
    pthread_mutex_unlock(&mutex);
}

typedef struct
{
    int row;
    int column;
} parameters;

int sudoku[9][9] =

    {
        {3, 5, 4, 6, 7, 8, 9, 1, 2},
        {6, 7, 2, 1, 9, 5, 3, 4, 8},
        {1, 9, 8, 3, 4, 2, 5, 6, 7},
        {8, 5, 9, 7, 6, 1, 4, 2, 3},
        {4, 2, 6, 8, 5, 3, 7, 9, 1},
        {7, 1, 3, 9, 2, 4, 8, 5, 6},
        {9, 6, 1, 5, 3, 7, 2, 8, 4},
        {2, 8, 7, 4, 1, 9, 6, 3, 5},
        {3, 4, 5, 2, 8, 6, 1, 7, 9}};

// {
//     {6, 2, 4, 5, 3, 9, 1, 8, 7},
//     {5, 1, 9, 7, 2, 8, 6, 3, 4},
//     {8, 3, 7, 6, 1, 4, 2, 9, 5},
//     {1, 4, 3, 8, 6, 5, 7, 2, 9},
//     {9, 5, 8, 2, 4, 7, 3, 6, 1},
//     {7, 6, 2, 3, 9, 1, 4, 5, 8},
//     {3, 7, 1, 9, 5, 6, 8, 4, 2},
//     {4, 9, 6, 1, 8, 2, 5, 7, 3},
//     {2, 8, 5, 4, 7, 3, 9, 1, 6}};

void *isColumnValid(void *param)
{
    parameters *params = (parameters *)param;
    int row = params->row;
    int col = params->column;
    if (row != 0 || col > 8)
    {
        fprintf(stderr, "Invalid row or column for col subsection! row=%d, col=%d\n", row, col);
        pthread_exit(NULL);
    }

    int validityArray[9] = {0};
    int i;
    for (i = 0; i < 9; i++)
    {
        int num = sudoku[i][col];
        if (num < 1 || num > 9 || validityArray[num - 1] == 1)
        {
            pthread_exit(NULL);
        }
        else
        {
            validityArray[num - 1] = 1;
        }
    }
    isValid();
    pthread_exit(NULL);
}

void *isRowValid(void *param)
{
    parameters *params = (parameters *)param;
    int row = params->row;
    int col = params->column;
    if (col != 0 || row > 8)
    {
        fprintf(stderr, "Invalid row or column for row subsection! row=%d, col=%d\n", row, col);
        pthread_exit(NULL);
    }

    int validityArray[9] = {0};
    int i;
    for (i = 0; i < 9; i++)
    {

        int num = sudoku[row][i];
        if (num < 1 || num > 9 || validityArray[num - 1] == 1)
        {
            pthread_exit(NULL);
        }
        else
        {
            validityArray[num - 1] = 1;
        }
    }
    isValid();
    pthread_exit(NULL);
}

void *is3x3Valid(void *param)
{
    parameters *params = (parameters *)param;
    int row = params->row;
    int col = params->column;
    if (row > 6 || row % 3 != 0 || col > 6 || col % 3 != 0)
    {
        fprintf(stderr, "Invalid row or column for subsection! row=%d, col=%d\n", row, col);
        pthread_exit(NULL);
    }
    int validityArray[9] = {0};
    int i, j;
    for (i = row; i < row + 3; i++)
    {
        for (j = col; j < col + 3; j++)
        {
            int num = sudoku[i][j];
            if (num < 1 || num > 9 || validityArray[num - 1] == 1)
            {
                pthread_exit(NULL);
            }
            else
            {
                validityArray[num - 1] = 1;
            }
        }
    }

    isValid();
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{

    num_threads = strtol(argv[1], NULL, 10);
    pthread_t threads[num_threads];

    int threadIndex = 0;
    int i, j;
    pthread_mutex_init(&mutex, NULL);

    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            if (i % 3 == 0 && j % 3 == 0)
            {
                parameters *data = (parameters *)malloc(sizeof(parameters));
                data->row = i;
                data->column = j;
                pthread_create(&threads[threadIndex++ % num_threads], NULL, is3x3Valid, data);
            }
            if (i == 0)
            {
                parameters *columnData = (parameters *)malloc(sizeof(parameters));
                columnData->row = i;
                columnData->column = j;
                pthread_create(&threads[threadIndex++ % num_threads], NULL, isColumnValid, columnData);
            }
            if (j == 0)
            {
                parameters *rowData = (parameters *)malloc(sizeof(parameters));
                rowData->row = i;
                rowData->column = j;
                pthread_create(&threads[threadIndex++ % num_threads], NULL, isRowValid, rowData);
            }
        }
    }

    for (i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    printf("valid value is %d \n number of threads %d \n", valid, num_threads);
    if (valid != 27)
    {
        printf("Sudoku solution is invalid!\n");
        return EXIT_SUCCESS;
    }

    printf("Sudoku solution is valid!\n");
    return EXIT_SUCCESS;
}