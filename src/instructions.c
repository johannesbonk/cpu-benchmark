#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "cpuinfo.h"


int main(int argc, char **argv) {
    struct cpu_info *cpu_info = malloc(sizeof(*cpu_info)); //included from cpuinfo.h
    printf("This program prints a list of some supported instructions of your cpu\n");

    set_cpu_info(cpu_info);
    if(cpu_info->POPCNT) {
        printf("POPCNT\n");
    }
    if(cpu_info->HYP_THR) {
        printf("HYP_THR\n");
    }
    if(cpu_info->X64) {
        printf("X64\n");
    }
    if(cpu_info->FPU) {
        printf("FPU\n");
    }
    if(cpu_info->PCMULQDQ) {
        printf("PCMULQDQ\n");
    }
    if(cpu_info->CX8) {
        printf("CX8\n");
    }
    if(cpu_info->CX16) {
        printf("CX16\n");
    }
    if(cpu_info->AVX512VPOPCNTDQ) {
        printf("AVX512VPOPCNTDQ\n");
    }


    free(cpu_info);

    return 0;
}