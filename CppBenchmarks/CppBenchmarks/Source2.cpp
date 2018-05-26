#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <thread>
#include <vector>
#define SIZE 1000
#define THREAD_SLEEP 50

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
		for (int j = 0; j < SIZE; j++) {
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

void sleep_foo() {
	cout << "[thread sleep foo started\n";
	this_thread::sleep_for(milliseconds(THREAD_SLEEP));
	cout << "[thread sleep ended\n";
}

milliseconds measure_context_switch(int n) {
	vector<thread> threads;

	Clock::time_point start = Clock::now();
	for (int i = 0; i < n; i++) {
		threads.push_back(thread(sleep_foo));
	}
	Clock::time_point end = Clock::now();
	for (auto& t : threads)
		t.join();

	milliseconds elapsed = (end - start)/n;
	return elapsed;
}

void loop() {
	int x;
	while (1)
		x = 1;
}


//milliseconds context_switch() {
//	Clock::time_point start1 = Clock::now();
//	thread main_t(loop);
//	//create thread which calls sleep, triggering a context switch
//	thread t1(sleep_foo);
//
//}


int main() {
	int n = SIZE;

	cout << "STATICMEM, Cpp, " << measure_static_mem_allocation(n).count() << " ms\n";
	cout << "DYNAMICMEM, Cpp,  " << measure_dynamic_mem_allocation(n).count() << " ms\n";
	cout << "MEMACCESS, Cpp, " << measure_memory_access(n).count() << " ms\n";
	cout << "THREADCREAT, Cpp, " << measure_thread_creation(n).count() << " ms\n"; 
	cout << "CONTEXTSW, Cpp, " << measure_context_switch(10).count() << " ms\n";

	return 0;
}