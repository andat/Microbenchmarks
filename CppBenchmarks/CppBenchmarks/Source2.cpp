#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <thread>
#include <vector>
#include <fstream>
#include <Windows.h>
#include <iomanip>
#define SIZE 1000
#define THREAD_SLEEP 10

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::duration<double, std::milli> milliseconds;
typedef std::chrono::duration<double, std::micro> microseconds;



using namespace std;
using std::chrono::duration_cast;


microseconds measure_static_mem_allocation(int n) {
	Clock::time_point start = Clock::now();

	for (int i = 0; i < n; i++) {
		int array[SIZE] = {0};
	}

	Clock::time_point end = Clock::now();
	microseconds elapsed = end - start;
	return elapsed / n;
}

microseconds measure_dynamic_mem_allocation(int n) {
	Clock::time_point start = Clock::now();

	for (int i = 0; i < n; i++) {
		int* array = new int[SIZE];
	}

	Clock::time_point end = Clock::now();
	microseconds elapsed = end - start;
	return elapsed / n;
}

microseconds measure_memory_access(int n) {
	int arr[SIZE] = {};

	Clock::time_point start = Clock::now();
	for(int i = 0; i < n; i++)
		for (int j = 0; j < SIZE; j++) {
			arr[j]++;
		}
	Clock::time_point end = Clock::now();

	microseconds elapsed = end - start;
	return elapsed / n;
}


void thread_foo() {
	int arr[SIZE] = {};
	for (int j = 0; j < SIZE; j++) {
			arr[j] = 1;
	}
}

microseconds thread_creation() {
	Clock::time_point f0 = Clock::now();
	thread_foo();
	Clock::time_point f1 = Clock::now();
	microseconds fduration = f1 - f0;
	//cout << "f dur: " << fduration.count();

	Clock::time_point start = Clock::now();
	thread t(thread_foo);
	t.join();
	Clock::time_point end = Clock::now();
	//cout << "\nt total: " << (end - start).count();

	microseconds elapsed = end - start;
	return elapsed - fduration;
}

microseconds measure_thread_creation(int n) {
	microseconds t10ms(10);
	for (int i = 0; i < n; i++) {
		t10ms += thread_creation();
	}
	return (t10ms - microseconds(10)) / n;
}

Clock::time_point sleep_end;

void sleep_foo() {
	//cout << "[thread sleep foo started\n";
	this_thread::sleep_for(microseconds(THREAD_SLEEP));
	sleep_end = Clock::now();
	//cout << "[thread sleep ended\n";
}

microseconds measure_context_switch1(int n) {
	vector<thread> threads;

	Clock::time_point start = Clock::now();
	for (int i = 0; i < n; i++) {
		threads.push_back(thread(sleep_foo));
	}
	for (auto& t : threads)
		t.join();
	Clock::time_point end = Clock::now();

	microseconds elapsed = (end - start - microseconds(THREAD_SLEEP)) / n;
	return elapsed;
}

microseconds context_switch() {
	Clock::time_point start = Clock::now();
	thread t(sleep_foo);
	t.join();

	microseconds elapsed = sleep_end - start;
	cout << elapsed.count() << "\n";
	return elapsed - microseconds(THREAD_SLEEP);
}

microseconds measure_context_switch2(int n) {
	microseconds creation_time = measure_thread_creation(n);

	microseconds t10ms(10);
	for (int i = 0; i < n; i++) {
		t10ms += context_switch() - creation_time;
	}
	return (t10ms - microseconds(10)) / n;
}

LARGE_INTEGER start, endt;

static void context_foo()
{
	QueryPerformanceCounter(&endt);
}

double context_switch3() {
	LARGE_INTEGER frequency;
	double duration;

	HANDLE thread;
	DWORD thread_id;
	QueryPerformanceFrequency(&frequency);

	thread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE) context_foo, 0, CREATE_SUSPENDED, &thread_id);
	QueryPerformanceCounter(&start);
	// set high priority so that the thread is resumed as soon as possible
	SetThreadPriority(thread, 10);
	ResumeThread(thread);
	//wait until thread executes
	WaitForSingleObject(thread, INFINITE);
	duration = (double)(endt.QuadPart - start.QuadPart) * 1000000 / frequency.QuadPart;
	return duration;
}

double measure_context_switch3(int n) {
	double total = 0.0;
	for (int i = 0; i< n; i++) {
		total += context_switch3();
	}
	return total / n;
}


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
	f << "STATICMEM, Cpp, " << std::fixed << ::setprecision(3) << measure_static_mem_allocation(n).count() << " us\n";
	f << "DYNAMICMEM, Cpp,  " << std::fixed << std::setprecision(3) << measure_dynamic_mem_allocation(n).count() << " us\n";
	f << "MEMACCESS, Cpp, " << std::fixed << std::setprecision(3) << measure_memory_access(n).count() << " us\n";
	f << "THREADCREAT, Cpp, " << std::fixed << std::setprecision(3) << measure_thread_creation(n).count() << " us\n";
	f << "CONTEXTSW, Cpp, " << std::fixed << std::setprecision(3) << measure_context_switch3(n)<< " us\n";

	cout << "Cpp benchmarks results written to file!\n";
	f.close();
	return 0;
}