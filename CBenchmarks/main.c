//#include <stdio.h>
//#include <stdlib.h>
//#include <cpuid.h>
//#include <x86intrin.h>
//#define SIZE 1000
//#define rdtsc __asm __emit 0fh __asm __emit 031h
//#define cpuid __asm __emit 0fh __asm __emit 0a2h
//
//unsigned cycles_high1 = 0, cycles_low1 = 0;
//unsigned cycles_high2 = 0, cycles_low2 = 0;
//unsigned __int64 temp_cycles1 = 0, temp_cycles2 = 0;
//unsigned __int64 registers[4];
//__int64 total_cycles = 0;
//
//
//int* measure_static_memory_allocation(){
//    __asm__(
//        "pushad;"
//        "CPUID;"
//        "RDTSC;"
//        "mov cycles_high1, %edx;"
//        "mov cycles_low1, %eax;"
//        "popad;"
//    );
//    __get_cpuid(0, registers[0], registers[1], registers[2], registers[3]);
//    __rdtsc();
//    int array[SIZE];
//
//    __asm__(
//        "pushad;"
//        "CPUID;"
//        "RDTSC;"
//        "mov cycles_high1, %edx;"
//        "mov cycles_low1, %eax;"
//        "popad;"
//    );
//    return *array;
//}
//
//int* measure_dynamic_memory_allocation(){
////    __asm {
////    pushad
////    CPUID
////    RDTSC
////    mov cycles_high1, edx
////    mov cycles_low1, eax
////    popad
////    }
//    int* array = (int*)malloc(SIZE * sizeof(int));
////    __asm {
////    pushad
////    CPUID
////    RDTSC
////    mov cycles_high2, edx
////    mov cycles_low2, eax
////    popad
////    }
////
////    temp_cycles1 = ((unsigned __int64)cycles_high1 << 32) | cycles_low1;
////    temp_cycles2 = ((unsigned __int64)cycles_high2 << 32) | cycles_low2;
////    total_cycles = temp_cycles2 - temp_cycles1;
//
//    printf("Dynamic memory allocation time: %d", total_cycles);
//    return array;
//}
//
//void measure_memory_access(){
//
//}
//
//int main() {
//    measure_static_memory_allocation();
//    return 0;
//}