#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <math.h>
#include <pthread.h>

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
    interval = (double) (end.QuadPart - start.QuadPart) * 1000/ frequency.QuadPart;
    return interval / n;

}

double measure_dynamic_mem_allocation(int n) {
    //struct timespec start, end;
    //clock_gettime(CLOCK_MONOTONIC, &start);

    LARGE_INTEGER frequency, start, end;
    double interval;

    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
    for (int i = 0; i < n; i++) {
        int* array = (int*)malloc(SIZE * sizeof(int));
    }

    QueryPerformanceCounter(&end);
    interval = (double) (end.QuadPart - start.QuadPart) * 1000/ frequency.QuadPart;
    return interval / n;
    //clock_gettime(CLOCK_MONOTONIC, &end);
    //return (end.tv_nsec - start.tv_nsec) / (double)n;
}

double measure_memory_access(int n) {
    int arr[SIZE];
    for(int i = 0; i< SIZE; i++)
        arr[i] = 0;

    LARGE_INTEGER frequency, start, end;
    double interval;

    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
    for(int i = 0; i < n; i++)
        for (int j = 0; j < SIZE;j++) {
            arr[j]++;
        }

    QueryPerformanceCounter(&end);
    interval = (double) (end.QuadPart - start.QuadPart) * 1000/ frequency.QuadPart;
    return interval / n;
}

void foo() {
    int arr[SIZE] = {};
    for (int j = 0; j < SIZE; j++) {
        arr[j] = 1;
    }
}

double measure_thread_creation(){
    LARGE_INTEGER frequency, start, end, foo_start, foo_end;
    double foo_time, interval;

    QueryPerformanceFrequency(&frequency);
    //measure time needed to execute foo
    QueryPerformanceCounter(&foo_start);
    foo();
    QueryPerformanceCounter(&foo_end);
    foo_time = (double) (foo_end.QuadPart - foo_start.QuadPart) * 1000/ frequency.QuadPart;

    //measure thread creation
    HANDLE thread_handle;
    DWORD thread_id;

    QueryPerformanceCounter(&start);
    thread_handle = CreateThread(0, 0, foo, (LPVOID)5, 0, &thread_id);
    QueryPerformanceCounter(&end);
    CloseHandle(thread_handle);

    interval = (double) (end.QuadPart - start.QuadPart) * 1000/ frequency.QuadPart;
    return interval - foo_time;
}

LARGE_INTEGER start, end;

void context_foo()
{
    QueryPerformanceCounter(&end);
}

double context_switch(){
    LARGE_INTEGER frequency;
    double duration;

    HANDLE thread;
    DWORD thread_id;
    QueryPerformanceFrequency(&frequency);

    thread = CreateThread(0,0, context_foo, 0, CREATE_SUSPENDED, &thread_id);
    QueryPerformanceCounter(&start);
    // set high priority so that the thread is resumed as soon as possible
    SetThreadPriority(thread, 10);
    ResumeThread(thread);
    //wait until thread executes
    Sleep(15);
    duration = (double) (end.QuadPart - start.QuadPart) * 1000/ frequency.QuadPart;
    //printf("\nduration: %.3lf", duration);
    return duration;
}

double measure_context_switch1(int n){
    double total = 0.0;
    for(int i = 0; i< n; i++){
        total += context_switch();
    }
    return total / n;
}

void thread_migration(){

}

double measure_thread_migration(){

}



int main() {
    int n = 1000;

    printf("STATICMEM, C, %lf ms\n", measure_static_mem_allocation(n));
    printf("DYNAMICMEM, C, %lf ms\n", measure_dynamic_mem_allocation(n));
    printf("MEMACCESS, C, %lf ms\n",measure_memory_access(n));
    printf("THREADCREAT, C, %lf ms\n", measure_thread_creation());
    printf("CONTEXTSW, C, %lf ms\n", measure_context_switch1(n));
    //printf("THREADMIGR, C, %.4lf ms\n", );
    return 0;
}

