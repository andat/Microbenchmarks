#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <thread>
#define SIZE 1000

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::duration<double, std::milli> milliseconds;
typedef std::chrono::nanoseconds nanoseconds;


using namespace std;
using std::chrono::duration_cast;

milliseconds measure_static_mem_allocation(int n) {
	Clock::time_point start = Clock::now();

	for (int i = 0; i < n; i++) {
		int array[SIZE] = {};
	}
	
	Clock::time_point end = Clock::now();
	milliseconds elapsed = end - start;
	return elapsed / n;
}

milliseconds measure_dynamic_mem_allocation(int n) {
	Clock::time_point start = Clock::now();

	for (int i = 0; i < n; i++) {
		int* array = new int[SIZE];
	}

	Clock::time_point end = Clock::now();
	milliseconds elapsed = end - start;
	return elapsed / n;
}

milliseconds measure_memory_access(int n) {
	int arr[SIZE] = {};

	Clock::time_point start = Clock::now();

	for(int i = 0; i < n; i++)
		for (int j = 0; j < SIZE;j++) {
			arr[j]++;
		}

	Clock::time_point end = Clock::now();
	milliseconds elapsed = end - start;
	return elapsed / n;
}


void thread_foo() {
	int arr[SIZE] = {};
	for (int j = 0; j < SIZE; j++) {
			arr[j] = 1;
	}
}

milliseconds thread_creation() {
	Clock::time_point f0 = Clock::now();
	thread_foo();
	Clock::time_point f1 = Clock::now();
	milliseconds fduration = f1 - f0;
	//cout << "f dur: " << fduration.count();

	Clock::time_point start = Clock::now();
	thread t(thread_foo);
	Clock::time_point end = Clock::now();
	t.join();
	//cout << "\nt total: " << (end - start).count();

	milliseconds elapsed = end - start;
	return elapsed - fduration;
}

milliseconds measure_thread_creation(int n) {
	milliseconds t10ms(10);
	for (int i = 0; i < n; i++) {
		t10ms += thread_creation();
	}
	return (t10ms - milliseconds(10)) / n;
}


int main() {
	int n = SIZE;

	cout << "STATICMEM, Cpp, " << measure_static_mem_allocation(n).count() << " ms\n";
	cout << "DYNAMICMEM, Cpp,  " << measure_dynamic_mem_allocation(n).count() << " ms\n";
	cout << "MEMACCESS, Cpp, " << measure_memory_access(n).count() << " ms\n";
	cout << "THREADCREAT, Cpp, " << measure_thread_creation(n).count() << " ms\n";

	return 0;
}