#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <thread>
#define SIZE 1000

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::milliseconds milliseconds;
typedef std::chrono::nanoseconds nanoseconds;

using namespace std;

nanoseconds measure_static_mem_allocation(int n) {
	Clock::time_point start = Clock::now();

	for (int i = 0; i < n; i++) {
		int array[SIZE] = {};
	}
	
	Clock::time_point end = Clock::now();
	std::chrono::duration<long, nano> elapsed = end - start;
	return elapsed / n;
}

nanoseconds measure_dynamic_mem_allocation(int n) {
	Clock::time_point start = Clock::now();

	for (int i = 0; i < n; i++) {
		int* array = new int[SIZE];
	}

	Clock::time_point end = Clock::now();
	std::chrono::duration<long, nano> elapsed = end - start;
	return elapsed / n;
}

nanoseconds measure_memory_access(int n) {
	int arr[SIZE] = {};

	Clock::time_point start = Clock::now();

	for(int i = 0; i < n; i++)
		for (int j = 0; j < SIZE;j++) {
			arr[j]++;
		}

	Clock::time_point end = Clock::now();
	std::chrono::duration<long, nano> elapsed = end - start;
	return elapsed / n;
}


void thread_foo() {
	int x = 0;
}

nanoseconds measure_thread_creation() {
	/*Clock::time_point f0 = Clock::now();
	thread_foo();
	Clock::time_point f1 = Clock::now();*/

	Clock::time_point start = Clock::now();
	thread t(thread_foo);
	Clock::time_point end = Clock::now();

	std::chrono::duration<long, nano> elapsed = end - start;// -n * (f1 - f0);
	return elapsed;
}

int main() {
	int n = 100;

	cout << "Static memory allocation time: " << measure_static_mem_allocation(n).count() << " ns\n";
	cout << "Dynamic memory allocation time: " << measure_dynamic_mem_allocation(n).count() << " ns\n";
	cout << "Memory access time: " << measure_memory_access(n).count() << " ns\n";
	//cout << "Thread creation time: " << measure_thread_creation().count() << "ns\n";

	return 0;
}