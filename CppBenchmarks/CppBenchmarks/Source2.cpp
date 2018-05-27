#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <thread>
#include <vector>
#include <fstream>
#include <Windows.h>
#define SIZE 1000
#define THREAD_SLEEP 50

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::duration<double, std::milli> milliseconds;
typedef std::chrono::duration<double, std::micro> microseconds;



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
	t.join();
	Clock::time_point end = Clock::now();
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
	//cout << "[thread sleep foo started\n";
	this_thread::sleep_for(milliseconds(THREAD_SLEEP));
	//cout << "[thread sleep ended\n";
}

milliseconds measure_context_switch1(int n) {
	vector<thread> threads;

	Clock::time_point start = Clock::now();
	for (int i = 0; i < n; i++) {
		threads.push_back(thread(sleep_foo));
	}
	for (auto& t : threads)
		t.join();
	Clock::time_point end = Clock::now();

	milliseconds elapsed = (end - start - milliseconds(THREAD_SLEEP)) / n;
	return elapsed;
}

milliseconds context_switch() {
	Clock::time_point start = Clock::now();
	thread t(sleep_foo);
	Clock::time_point end = Clock::now();
	t.join();

	milliseconds elapsed = (end - start - milliseconds(THREAD_SLEEP));
	return elapsed;
}

milliseconds measure_context_switch2(int n) {
	milliseconds creation_time = measure_thread_creation(n);

	milliseconds t10ms(10);
	for (int i = 0; i < n; i++) {
		t10ms += context_switch() - creation_time;
	}
	return (t10ms - milliseconds(10)) / n;
}

//LARGE_INTEGER start, endt;
//
//static void context_foo()
//{
//	QueryPerformanceCounter(&endt);
//}
//
//double context_switch() {
//	LARGE_INTEGER frequency;
//	double duration;
//
//	HANDLE thread;
//	DWORD thread_id;
//	QueryPerformanceFrequency(&frequency);
//
//	thread = CreateThread(0, 0, &context_foo, 0, CREATE_SUSPENDED, &thread_id);
//	QueryPerformanceCounter(&start);
//	// set high priority so that the thread is resumed as soon as possible
//	SetThreadPriority(thread, 10);
//	ResumeThread(thread);
//	//wait until thread executes
//	Sleep(15);
//	duration = (double)(endt.QuadPart - start.QuadPart) * 1000 / frequency.QuadPart;
//	//printf("\nduration: %.3lf", duration);
//	return duration;
//}
//
//double measure_context_switch2(int n) {
//	double total = 0.0;
//	for (int i = 0; i< n; i++) {
//		total += context_switch();
//	}
//	return total / n;
//}
//
//void loop() {
//	int x;
//	while (1)
//		x = 1;
//}
//
//
////milliseconds context_switch() {
////	Clock::time_point start1 = Clock::now();
////	thread main_t(loop);
////	//create thread which calls sleep, triggering a context switch
////	thread t1(sleep_foo);
////
////}


int main(int argc, char *argv[]) {
	int n;
	if (argc < 2) {
		n = 1000;
	}
	else
		n = atoi(argv[1]);

	ofstream f;
	f.open("C:/Users/zenbookx/Documents/Facultate/An III/Sem II/SCS/project/CppBenchmarks/CppBenchmarks/cpp_benchmarks_results.csv");
	
	f << n << "\n";
	f << "STATICMEM, Cpp, " << measure_static_mem_allocation(n).count() << " ms\n";
	f << "DYNAMICMEM, Cpp,  " << measure_dynamic_mem_allocation(n).count() << " ms\n";
	f << "MEMACCESS, Cpp, " << measure_memory_access(n).count() << " ms\n";
	f << "THREADCREAT, Cpp, " << measure_thread_creation(n).count() << " ms\n"; 
	f << "CONTEXTSW, Cpp, " << measure_context_switch1(n).count() << " ms\n";

	cout << "Cpp benchmarks results written to file!\n";
	f.close();
	return 0;
}