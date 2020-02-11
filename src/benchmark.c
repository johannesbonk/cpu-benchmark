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

#define MEASUREMENT_ITERATIONS 10000
#define STARTUP_PROMPT_ROWS 5
#define STARTUP_PROMPT_COLUMNS 81

//***********************************************
//************GLOBAL_VARIABLES**************
//***********************************************
char startup_prompt[STARTUP_PROMPT_ROWS][STARTUP_PROMPT_COLUMNS] = {
"   __________  __  __      ____                  __                         __",
"  / ____/ __ \\/ / / /     / __ )___  ____  _____/ /_  ____ ___  ____ ______/ /__ ",
" / /   / /_/ / / / /_____/ __  / _ \\/ __ \\/ ___/ __ \\/ __ `__ \\/ __ `/ ___/ //_/",
"/ /___/ ____/ /_/ /_____/ /_/ /  __/ / / / /__/ / / / / / / / / /_/ / /  / ,<",
"\\____/_/    \\____/     /_____/\\___/_/ /_/\\___/_/ /_/_/ /_/ /_/\\__,_/_/  /_/|_|"};

//***********************************************
//************HELPER_FUNCTIONS**************
//***********************************************

static char *random_string(char *str, size_t size) {
    const char charset[] = "0123456789";
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
    const size_t string_cnt = 8;
    const size_t string_len = 16;
    uint64_t fir_str_num[string_cnt];
    uint64_t sec_str_num[string_cnt];
    uint64_t fir_str_hw[string_cnt];
    uint64_t sec_str_hw[string_cnt];
    uint64_t fir_str_lz[string_cnt];
    uint64_t sec_str_lz[string_cnt];
    uint64_t hamming_distance[string_cnt];
    uint64_t hamming_distance_lz[string_cnt];
    uint64_t temp[string_cnt];

    for(uint32_t i=0; i<MEASUREMENT_ITERATIONS; i++) {
        const char fir_str[string_cnt][string_len];
        const char sec_str[string_cnt][string_len];
        for(uint8_t k=0; k<string_cnt; k++) {
            random_string((char *)&fir_str[k], string_len);
            random_string((char *)&sec_str[k], string_len);

            fir_str_num[k] = strtol(fir_str[k], NULL, 10);
            if(fir_str_num[k] == 0) {
                if(errno == EINVAL) {
                    printf("Conversion error occurred: %d\n", errno);
                }
                if (errno == ERANGE) {
                    printf("The value provided was out of range\n");
                }
            }
            sec_str_num[k] = strtol(sec_str[k], NULL, 10);
            if(sec_str_num[k] == 0) {
                if(errno == EINVAL) {
                    printf("Conversion error occurred: %d\n", errno);
                }
                if (errno == ERANGE) {
                    printf("The value provided was out of range\n");
                }
            }
        }

        for(uint8_t h=0; h<string_cnt; h++) {
            gettimeofday(&start_time, NULL);

            if(abm_supported || popcnt_supported) {
                //Hamming Weight calculation
                __asm__ volatile ("popcntq %2, %0 \n\t"  //Hamming Weight of first string
                                  "popcntq %3, %1 \n\t" //Hamming Weight of second string
                    :"=r"(fir_str_hw[h]), "=r"(sec_str_hw[h])
                    :"r"(fir_str_num[h]), "r"(sec_str_num[h])
                    :
                );

                __asm__ volatile ("popcntq %1, %0"  //Hamming Wight of second string
                    :"=r"(sec_str_hw[h])
                    :"r"(sec_str_num[h])
                    :
                );

                //Hamming Distance of first string and second string
                //Hamming Distance calculation HammingDistance(bitstring, bitstring2) == HammingWeight(bitstring ^ bitstring2)
                temp[h] = fir_str_num[h] ^ sec_str_num[h];
                __asm__ volatile ("popcntq %1, %0"
                    :"=r"(hamming_distance[h])
                    :"r"(temp[h])
                    :
                );
            } else {
                //Hamming Weight calculation
                fir_str_hw[h] = emulated_popcnt(fir_str_num[h]); //Hamming Weight of first string
                sec_str_hw[h] = emulated_popcnt(sec_str_num[h]); //Hamming Wight of second string

                //Hamming Distance of first string and second string
                //Hamming Distance calculation HammingDistance(bitstring, bitstring2) == HammingWeight(bitstring ^ bitstring2)
                temp[h] = fir_str_num[h] ^ sec_str_num[h];
                hamming_distance[h] = emulated_popcnt(temp[h]);
            }

            if(abm_supported) {
                //leading zeros calculation with LZCNT
                __asm__ volatile ("lzcntq %0, %3 \n\t"
                                  "lzcntq %1, %4 \n\t"
                                  "lzcntq %2, %5 \n\t"
                    :"=r"(fir_str_lz[h]), "=r"(sec_str_lz[h]), "=r"(hamming_distance_lz[h])
                    :"r"(fir_str_num[h]), "r"(sec_str_num[h]), "r" (hamming_distance[h])
                    :
                );

            } else {
                //calculation of leading zeros with LZCNT
                fir_str_lz[h] = emulated_lzcnt(fir_str_num[h]); //leading zeros count of first string bitstring
                sec_str_lz[h] = emulated_lzcnt(sec_str_num[h]); //leading zeros count of second string bitstring
                hamming_distance_lz[h] = emulated_lzcnt(hamming_distance[h]); //leading zeros count of hamming distance
            }

            gettimeofday(&end_time, NULL);
            time_sum += (double) (end_time.tv_usec - start_time.tv_usec);
        }
    }

    return (double) (time_sum/10);
}

int main(int argc, char *argv[]) {
    // print ASCII art at program entry
    for (char r = 0; r < STARTUP_PROMPT_ROWS; ++r){
      for(char c = 0; c < STARTUP_PROMPT_COLUMNS; ++c){
        printf("%c",startup_prompt[r][c]);
      }
      printf("\n");
    }

    struct cpu_info *cpu_info = malloc(sizeof(*cpu_info)); //included from cpuinfo.h
    struct execution_time execution_time; //included from cpuinfo.h

    set_cpu_info(cpu_info);

    execution_time.ABM = abm_bench(cpu_info->ABM, cpu_info->POPCNT);
    execution_time.POPCNT = abm_bench(false, cpu_info->POPCNT);
    char *em_str[2] = {" (emulated)", ""}; //string for output if bit not set
    printf("ABM%s execution time: %f \r\n", em_str[cpu_info->ABM],execution_time.ABM);
    printf("POPCNT%s execution time: %f \r\n", em_str[cpu_info->POPCNT], execution_time.POPCNT);

    //freeing all allocated memory
    free(cpu_info);
    return 0;
}
