#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <math.h>

#define SIZE 1000


double measure_static_mem_allocation(int n) {
    LARGE_INTEGER frequency, start, end;
    double interval;

    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);

    //clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < n; i++) {
        int array[SIZE] = {};
    }

    //clock_gettime(CLOCK_MONOTONIC, &end);
    //return (end.tv_nsec - start.tv_nsec) /(double)n;
    QueryPerformanceCounter(&end);
    interval = (double) (end.QuadPart - start.QuadPart) / frequency.QuadPart;
    interval *= pow(10, 9);
    return interval;

}

double measure_dynamic_mem_allocation(int n) {
    //struct timespec start, end;
    //clock_gettime(CLOCK_MONOTONIC, &start);

    LARGE_INTEGER frequency, start, end;
    double interval;

    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
    for (int i = 0; i < n; i++) {
        int* array = (int*)malloc(n * sizeof(int));
    }

    QueryPerformanceCounter(&end);
    interval = (double) (end.QuadPart - start.QuadPart) / frequency.QuadPart;
    interval *= pow(10, 9);
    return interval;
    //clock_gettime(CLOCK_MONOTONIC, &end);
    //return (end.tv_nsec - start.tv_nsec) / (double)n;
}

double measure_memory_access(int n) {
    int arr[SIZE];
    for(int i = 0; i< SIZE; i++)
        arr[i] = 0;

//    struct timespec start, end;
//    clock_gettime(CLOCK_MONOTONIC, &start);

    LARGE_INTEGER frequency, start, end;
    double interval;

    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
    for(int i = 0; i < n; i++)
        for (int j = 0; j < SIZE;j++) {
            arr[j]++;
        }

    QueryPerformanceCounter(&end);
    interval = (double) (end.QuadPart - start.QuadPart) / frequency.QuadPart;
    interval *= pow(10, 9);
    return interval;
    // clock_gettime(CLOCK_MONOTONIC, &end);
    //return (end.tv_nsec - start.tv_nsec) / (double)n;
}


int main() {
    int n = 100;

    printf("STATICMEM, C, %.2lf ns\n", measure_static_mem_allocation(n));
    printf("DYNAMICMEM, C, %.2lf ns\n", measure_dynamic_mem_allocation(n));
    printf("MEMACCESS, C, %.2lf ns\n",measure_memory_access(n));

    return 0;
}

