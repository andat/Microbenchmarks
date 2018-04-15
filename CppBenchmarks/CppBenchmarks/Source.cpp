//#include <stdio.h>
//#include <stdlib.h>
//#define rdtsc __asm __emit 0fh __asm __emit 031h
//#define cpuid __asm __emit 0fh __asm __emit 0a2h//////unsigned compute_cpuid_overhead() {//	unsigned cycles_high1 = 0, cycles_low1 = 0, cpuid_time = 0;
//	unsigned cycles_high2 = 0, cycles_low2 = 0;////	__asm 
//	{
//		pushad
//		CPUID
//		RDTSC
//		mov cycles_high1, edx
//		mov cycles_low1, eax
//		popad
//		pushad
//		CPUID
//		RDTSC
//		popad
//		pushad
//		CPUID
//		RDTSC
//		mov cycles_high1, edx
//		mov cycles_low1, eax
//		popad
//		pushad
//		CPUID
//		RDTSC
//		popad
//		pushad
//		CPUID
//		RDTSC
//		mov cycles_high1, edx
//		mov cycles_low1, eax
//		popad
//		pushad
//		CPUID
//		RDTSC
//		sub eax, cycles_low1
//		mov cpuid_time, eax
//		popad
//	}////	return cpuid_time;//}//
//_int64 measure_static_mem_allocation(int cpuid_time, int n) {
//	unsigned cycles_high1 = 0, cycles_low1 = 0;
//	unsigned cycles_high2 = 0, cycles_low2 = 0;
//	unsigned __int64 temp_cycles1 = 0, temp_cycles2 = 0;
//	__int64 total_cycles = 0;
//
//	//Measure the code sequence
//	__asm {
//		pushad
//		CPUID
//		RDTSC
//		mov cycles_high1, edx
//		mov cycles_low1, eax
//		popad
//	}
//
//	for (int  i= 0; i < n; i++)
//	{
//		int array[1000] = {};
//	}
//	
//	__asm {
//		pushad
//		CPUID
//		RDTSC
//		mov cycles_high2, edx
//		mov cycles_low2, eax
//		popad
//	}
//
//	temp_cycles1 = ((unsigned __int64)cycles_high1 << 32) | cycles_low1;
//	temp_cycles2 = ((unsigned __int64)cycles_high2 << 32) | cycles_low2;
//	total_cycles = temp_cycles2 - temp_cycles1 - cpuid_time;
//
//	printf("No. of cycles for static memory allocation: %d\n", total_cycles);
//
//	return total_cycles;
//}
//
//_int64 measure_dynamic_mem_allocation(int cpuid_time, int n) {
//	unsigned cycles_high1 = 0, cycles_low1 = 0;
//	unsigned cycles_high2 = 0, cycles_low2 = 0;
//	unsigned __int64 temp_cycles1 = 0, temp_cycles2 = 0;
//	__int64 total_cycles = 0;
//
//	//Measure the code sequence
//	__asm {
//		pushad
//		CPUID
//		RDTSC
//		mov cycles_high1, edx
//		mov cycles_low1, eax
//		popad
//	}
//
//	for (int i = 0; i < n; i++)
//	{
//		int *nums = (int*)malloc(sizeof(int*) * n);
//	}
//
//	__asm {
//		pushad
//		CPUID
//		RDTSC
//		mov cycles_high2, edx
//		mov cycles_low2, eax
//		popad
//	}
//
//	temp_cycles1 = ((unsigned __int64)cycles_high1 << 32) | cycles_low1;
//	temp_cycles2 = ((unsigned __int64)cycles_high2 << 32) | cycles_low2;
//	total_cycles = temp_cycles2 - temp_cycles1 - cpuid_time;
//
//	printf("No. of cycles for dynamic memory allocation: %d\n", total_cycles);
//	return total_cycles;
//}
//
//
//int main() {
//
//
//	//compute the CPUID overhead 
//	unsigned cpuid_time = compute_cpuid_overhead(); 
//
//	measure_static_mem_allocation(cpuid_time, 100);
//	
//
//	return 0;
//}