// MIT License
//
// Copyright (c) 2019 Johannes Bonk and Maximilian Ley
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <immintrin.h>
#include <sys/time.h>
#include "cpuinfo.h"

#define MEASUREMENT_ITERATIONS 10000

//***********************************************
//************EMULATED_INSTRUCTIONS**************
//***********************************************

uint64_t emulated_popcnt(uint64_t val) {
    const uint64_t c1  = 0x5555555555555555;
    const uint64_t c2  = 0x3333333333333333;
    const uint64_t c4  = 0x0f0f0f0f0f0f0f0f;
    const uint64_t h01 = 0x0101010101010101;

    val -= (val >> 1) & c1;
    val = (val & c2) + ((val >> 2) & c2);
    val = (val + (val >> 4)) & c4;
    return (val * h01) >> 56;
}

uint64_t emulated_lzcnt(uint64_t val) {
    int64_t signed_val = (int64_t) val;
    uint64_t zero_cnt = 0;
    const uint64_t bits = sizeof(signed_val) * 8;
    for (uint64_t i = bits; --i; ) {
        if (signed_val < 0) break;
        zero_cnt++;
        signed_val <<= 1;
    }
    return zero_cnt;
}

//***********************************************
//**************EXTENSIONS_BENCHES*+*************
//***********************************************

//returns execution time of ABM instructions (if not supported its emulated by an algorithm)
//instructions used: POPCNT; LZCNT
double abm_bench(bool supported) {
    double time_sum = 0.0;
    struct timeval start_time, end_time;
    uint64_t hello_world_num = 0;
    uint64_t hello_abm_num = 0;
    uint64_t hello_world_hw = 0;
    uint64_t hello_abm_hw = 0;
    uint64_t hello_world_lz = 0;
    uint64_t hello_abm_lz = 0;
    uint64_t hamming_distance = 0;
    uint64_t hamming_distance_lz = 0;
    uint64_t temp = 0;
    char *hello_world = "HelloWorld!";
    char *hello_abm = "HelloABM";
    char *string_end;

    hello_world_num = strtol(hello_world, &string_end, 10);
    if(string_end == hello_world) {
        printf("Could not convert '%s' to long and leftover is: '%s'\n", hello_world, string_end);
    }
    hello_abm_num = strtol(hello_abm, &string_end, 10);
    if(string_end == hello_abm) {
        printf("Could not convert '%s' to long and leftover is: '%s'\n", hello_world, string_end);
    }

    for(uint16_t i=0; i<MEASUREMENT_ITERATIONS; i++) {
        gettimeofday(&start_time, NULL);
        if(supported) {
            //Hamming Weight calculation
            __asm__ volatile ("popcntq %2, %0 \n\t"  //Hamming Weight of HelloWorld!
                              "popcntq %3, %1 \n\t" //Hamming Weight of HelloABM
                :"=r"(hello_world_hw), "=r"(hello_abm_hw)
                :"r"(hello_world_num), "r"(hello_abm_num)
                :
            );

            __asm__ volatile ("popcntq %1, %0"  //Hamming Wight of HelloABM
                :"=r"(hello_abm_hw)
                :"r"(hello_abm_num)
                :
            );

            //Hamming Distance of HelloWorld! and HelloABM
            //Hamming Distance calculation HammingDistance(bitstring, bitstring2) == HammingWeight(bitstring ^ bitstring2)
            temp = hello_world_num ^ hello_abm_num;
            __asm__ volatile ("popcntq %1, %0"
                :"=r"(hamming_distance)
                :"r"(temp)
                :
            );

            //leading zeros calculation with LZCNT
            __asm__ volatile ("lzcntq %0, %3 \n\t"
                              "lzcntq %1, %4 \n\t"
                              "lzcntq %2, %5 \n\t"
                :"=r"(hello_world_lz), "=r"(hello_abm_lz), "=r"(hamming_distance_lz)
                :"r"(hello_world_num), "r"(hello_abm_num), "r" (hamming_distance)
                :
            );

        } else {
            //Hamming Weight calculation
            hello_world_hw = emulated_popcnt(hello_world_num); //Hamming Weight of HelloWorld!
            hello_abm_hw = emulated_popcnt(hello_abm_num); //Hamming Wight of HelloABM

            //Hamming Distance of HelloWorld! and HelloABM
            //Hamming Distance calculation HammingDistance(bitstring, bitstring2) == HammingWeight(bitstring ^ bitstring2)
            temp = hello_world_num ^ hello_abm_num;
            hamming_distance = emulated_popcnt(temp);

            //calculation of leading zeros with LZCNT
            hello_world_lz = emulated_lzcnt(hello_world_num); //leading zeros count of HelloWorld! bitstring
            hello_abm_lz = emulated_lzcnt(hello_abm_num); //leading zeros count of HelloABM bitstring
            hamming_distance_lz = emulated_lzcnt(hamming_distance); //leading zeros count of hamming distance
        }
        gettimeofday(&end_time, NULL);
        time_sum += (double) (end_time.tv_usec - start_time.tv_usec);
    }

    return (double) (time_sum/10);
}

int main(int argc, char *argv[]) {
    struct cpu_info *cpu_info = malloc(sizeof(*cpu_info)); //included from cpuinfo.h
    struct execution_time execution_time; //included from cpuinfo.h

    set_cpu_info(cpu_info);

    execution_time.ABM = abm_bench(cpu_info->ABM);

    printf("ABM execution time: %f \r\n", execution_time.ABM);

    //freeing all allocated memory
    free(cpu_info);
    return 0;
}
