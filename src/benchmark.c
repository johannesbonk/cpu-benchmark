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
#include <errno.h>
#include "cpuinfo.h"

#define MEASUREMENT_ITERATIONS 10

//***********************************************
//************HELPER_FUNCTIONS**************
//***********************************************

static char *random_string(char *str, size_t size) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!,;.:?";
    if(size) {
        --size;
        for (size_t i=0; i<size; i++) {
            int symbol = rand() % (int) (sizeof charset - 1);
            str[i] = charset[symbol];
        }
        str[size] = '\0';
    }
    return str;
}

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
double abm_bench(bool abm_supported, bool popcnt_supported) {
    double time_sum = 0.0;
    struct timeval start_time, end_time;
    const size_t string_len = 2;
    uint64_t fir_str_num = 0;
    uint64_t sec_str_num = 0;
    uint64_t fir_str_hw = 0;
    uint64_t sec_str_hw = 0;
    uint64_t fir_str_lz = 0;
    uint64_t sec_str_lz = 0;
    uint64_t hamming_distance = 0;
    uint64_t hamming_distance_lz = 0;
    uint64_t temp = 0;
    char *fir_str = malloc(sizeof(char) * string_len);
    char *sec_str = malloc(sizeof(char) * string_len);

    for(uint16_t i=0; i<MEASUREMENT_ITERATIONS; i++) {
        random_string(fir_str, string_len);
        random_string(sec_str, string_len);

        fir_str_num = strtol(fir_str, NULL, 16);
        if(fir_str_num == 0) {
            if(errno == EINVAL) {
                printf("Conversion error occurred: %d\n", errno);
            }
            if (errno == ERANGE) {
                printf("The value provided was out of range\n");
            }
        }
        sec_str_num = strtol(sec_str, NULL, 10);
        if(sec_str_num == 0) {
            if(errno == EINVAL) {
                printf("Conversion error occurred: %d\n", errno);
            }
            if (errno == ERANGE) {
                printf("The value provided was out of range\n");
            }
        }

        gettimeofday(&start_time, NULL);
        if(abm_supported || popcnt_supported) {
            //Hamming Weight calculation
            __asm__ volatile ("popcntq %2, %0 \n\t"  //Hamming Weight of first string
                              "popcntq %3, %1 \n\t" //Hamming Weight of second string
                :"=r"(fir_str_hw), "=r"(sec_str_hw)
                :"r"(fir_str_num), "r"(sec_str_num)
                :
            );

            __asm__ volatile ("popcntq %1, %0"  //Hamming Wight of second string
                :"=r"(sec_str_hw)
                :"r"(sec_str_num)
                :
            );

            //Hamming Distance of first string and second string
            //Hamming Distance calculation HammingDistance(bitstring, bitstring2) == HammingWeight(bitstring ^ bitstring2)
            temp = fir_str_num ^ sec_str_num;
            __asm__ volatile ("popcntq %1, %0"
                :"=r"(hamming_distance)
                :"r"(temp)
                :
            );
        } else {
                //Hamming Weight calculation
                fir_str_hw = emulated_popcnt(fir_str_num); //Hamming Weight of first string
                sec_str_hw = emulated_popcnt(sec_str_num); //Hamming Wight of second string

                //Hamming Distance of first string and second string
                //Hamming Distance calculation HammingDistance(bitstring, bitstring2) == HammingWeight(bitstring ^ bitstring2)
                temp = fir_str_num ^ sec_str_num;
                hamming_distance = emulated_popcnt(temp);
        }
        if(abm_supported) {
            //leading zeros calculation with LZCNT
            __asm__ volatile ("lzcntq %0, %3 \n\t"
                              "lzcntq %1, %4 \n\t"
                              "lzcntq %2, %5 \n\t"
                :"=r"(fir_str_lz), "=r"(sec_str_lz), "=r"(hamming_distance_lz)
                :"r"(fir_str_num), "r"(sec_str_num), "r" (hamming_distance)
                :
            );

        } else {
            //calculation of leading zeros with LZCNT
            fir_str_lz = emulated_lzcnt(fir_str_num); //leading zeros count of first string bitstring
            sec_str_lz = emulated_lzcnt(sec_str_num); //leading zeros count of second string bitstring
            hamming_distance_lz = emulated_lzcnt(hamming_distance); //leading zeros count of hamming distance
        }
        gettimeofday(&end_time, NULL);
        time_sum += (double) (end_time.tv_usec - start_time.tv_usec);
    }

    free(fir_str);
    free(sec_str);
    return (double) (time_sum/10);
}

int main(int argc, char *argv[]) {
    struct cpu_info *cpu_info = malloc(sizeof(*cpu_info)); //included from cpuinfo.h
    struct execution_time execution_time; //included from cpuinfo.h

    set_cpu_info(cpu_info);

    execution_time.ABM = abm_bench(cpu_info->ABM, cpu_info->POPCNT);

    printf("ABM execution time: %f \r\n", execution_time.ABM);

    //freeing all allocated memory
    free(cpu_info);
    return 0;
}
