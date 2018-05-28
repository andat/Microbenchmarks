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

    for (int i = 0; i < n; i++) {
        int array[SIZE] = {0};
    }

    QueryPerformanceCounter(&end);
    interval = (double) (end.QuadPart - start.QuadPart) * 1000000/ frequency.QuadPart;
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

    interval = (double) (end.QuadPart - start.QuadPart) * 1000000/ frequency.QuadPart;
    return interval / n;
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
        for (int j = 0; j < SIZE; j++) {
            arr[j]++;
        }
    QueryPerformanceCounter(&end);

    interval = (double) (end.QuadPart - start.QuadPart) * 1000000/ frequency.QuadPart;
    return interval / n;
}

void foo() {
    int arr[SIZE] = {0};
    for (int j = 0; j < SIZE; j++) {
        arr[j] = 1;
    }
}

double thread_creation(){
    LARGE_INTEGER frequency, start, end, foo_start, foo_end;
    double foo_time, interval;

    QueryPerformanceFrequency(&frequency);
    //measure time needed to execute foo
    QueryPerformanceCounter(&foo_start);
    foo();
    QueryPerformanceCounter(&foo_end);
    foo_time = (double) (foo_end.QuadPart - foo_start.QuadPart) * 1000000/ frequency.QuadPart;

    //measure thread creation
    HANDLE thread_handle;
    DWORD thread_id;

    QueryPerformanceCounter(&start);
    thread_handle = CreateThread(0, 0, foo, (LPVOID)5, 0, &thread_id);
    WaitForSingleObject(thread_handle, INFINITE);
    QueryPerformanceCounter(&end);

    interval = (double) (end.QuadPart - start.QuadPart)  * 1000000/ frequency.QuadPart;
    return interval - foo_time;
}

double measure_thread_creation(int n){
    double total = 0.0;
    for(int i = 0; i<n; i++)
        total += thread_creation();
    return total / n;
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
    SetThreadPriority(thread, THREAD_PRIORITY_HIGHEST);
    ResumeThread(thread);
    //wait until thread executes
    WaitForSingleObject(thread, INFINITE);
    duration = (double) (end.QuadPart - start.QuadPart) * 1000000/ frequency.QuadPart;
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



int main(int argc, char*argv[]) {
    int n;
    if (argc < 2) {
        n = 1000;
    } else
        n = atoi(argv[1]);

    FILE *f = fopen("C:/Users/zenbookx/Documents/Facultate/An III/Sem II/SCS/project/CBenchmarks/cmake-build-debug/c_benchmarks_results.csv", "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    //print number of iterations
    fprintf(f, "%d\n", n);
    fprintf(f, "STATICMEM, C, %.3lf us\n", measure_static_mem_allocation(n));
    fprintf(f, "DYNAMICMEM, C, %.3lf us\n", measure_dynamic_mem_allocation(n));
    fprintf(f, "MEMACCESS, C, %.3lf us\n",measure_memory_access(n));
    fprintf(f, "THREADCREAT, C, %.3lf us\n", measure_thread_creation(n));
    fprintf(f, "CONTEXTSW, C, %.3lf us\n", measure_context_switch1(n));
    //printf("THREADMIGR, C, %.4lf ms\n", );

    printf("C benchmarks results written to file!");
    fclose(f);
    return 0;
}

