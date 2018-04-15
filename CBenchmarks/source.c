#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define SIZE 1000


float measure_static_mem_allocation(int n) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < n; i++) {
        int array[SIZE] = {};
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    return (end.tv_nsec - start.tv_nsec) /(float)n;
}

float measure_dynamic_mem_allocation(int n) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < n; i++) {
        int* array = (int*)malloc(n * sizeof(int));
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    return (end.tv_nsec - start.tv_nsec) / (float)n;
}

float measure_memory_access(int n) {
    int arr[SIZE];
    for(int i = 0; i< SIZE; i++)
        arr[i] = 0;

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    for(int i = 0; i < n; i++)
        for (int j = 0; j < SIZE;j++) {
            arr[j]++;
        }

    clock_gettime(CLOCK_MONOTONIC, &end);
    return (end.tv_nsec - start.tv_nsec) / (float)n;
}


int main() {
    int n = 100;

    printf("Static memory allocation time: %.2f\n", measure_static_mem_allocation(n));
    printf("Dynamic memory allocation time: %.2f\n", measure_dynamic_mem_allocation(n));
    printf("Memory access time: %.2f \n",measure_memory_access(n));

    return 0;
}

