// MIT License
//
// Copyright (c) 2019 Johannes Bonk and Maximilian Ley
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <cpuid.h>
#include "cpuinfo.h"

//returns NULL pointer on failure
void set_cpu_info(struct cpu_info *info) {
    uint32_t values[4];

    if (__get_cpuid(0x00000001, &values[0], &values[1], &values[2], &values[3])){
        info->SSE3      = (values[2] & ((uint32_t)1 <<  0)) != 0;
        info->PCMULQDQ  = (values[2] & ((uint32_t)1 <<  1)) != 0;
        info->SMX       = (values[2] & ((uint32_t)1 <<  6)) != 0;
        info->SSSE3     = (values[2] & ((uint32_t)1 <<  9)) != 0;
        info->FMA3      = (values[2] & ((uint32_t)1 << 12)) != 0;
        info->CX16      = (values[2] & ((uint32_t)1 << 13)) != 0;
        info->SSE41     = (values[2] & ((uint32_t)1 << 19)) != 0;
        info->SSE42     = (values[2] & ((uint32_t)1 << 20)) != 0;
        info->MOVBE     = (values[2] & ((uint32_t)1 << 22)) != 0;
        info->POPCNT    = (values[2] & ((uint32_t)1 << 23)) != 0;
        info->AES       = (values[2] & ((uint32_t)1 << 25)) != 0;
        info->XSAVE     = (values[2] & ((uint32_t)1 << 26)) != 0;
        info->AVX       = (values[2] & ((uint32_t)1 << 28)) != 0;
        info->F16       = (values[2] & ((uint32_t)1 << 29)) != 0;
        info->RDRAND    = (values[2] & ((uint32_t)1 << 30)) != 0;
        info->FPU       = (values[3] & ((uint32_t)1 <<  0)) != 0;
        info->CX8       = (values[3] & ((uint32_t)1 <<  8)) != 0;
        info->FCMOV     = (values[3] & ((uint32_t)1 << 15)) != 0;
        info->MMX       = (values[3] & ((uint32_t)1 << 23)) != 0;
        info->FXSR      = (values[3] & ((uint32_t)1 << 24)) != 0;
        info->SSE       = (values[3] & ((uint32_t)1 << 25)) != 0;
        info->SSE2      = (values[3] & ((uint32_t)1 << 26)) != 0;
        info->HYP_THR   = (values[3] & ((uint32_t)1 << 28)) != 0;
    } else {
        info = NULL; //adressing went wrong since no access to basic extensions
        return;
    }

    if (__get_cpuid(0x00000007, &values[0], &values[1], &values[2], &values[3])){
        info->SGX               = (values[1] & ((uint32_t)1 <<  2)) != 0;
        info->BMI1              = (values[1] & ((uint32_t)1 <<  3)) != 0;
        info->TSX               = (values[1] & ((uint32_t)1 <<  4)) != 0;
        info->AVX2              = (values[1] & ((uint32_t)1 <<  5)) != 0;
        info->BMI2              = (values[1] & ((uint32_t)1 <<  8)) != 0;
        info->ERMS              = (values[1] & ((uint32_t)1 <<  9)) != 0;
        info->INVPCID           = (values[1] & ((uint32_t)1 << 10)) != 0;
        info->INTEL_MPX         = (values[1] & ((uint32_t)1 << 14)) != 0;
        info->AVX512F           = (values[1] & ((uint32_t)1 << 16)) != 0;
        info->AVX512DQ          = (values[1] & ((uint32_t)1 << 17)) != 0;
        info->RDSEED            = (values[1] & ((uint32_t)1 << 18)) != 0;
        info->INTEL_ADX         = (values[1] & ((uint32_t)1 << 19)) != 0;
        info->AVX512IFMA        = (values[1] & ((uint32_t)1 << 21)) != 0;
        info->PCOMMIT           = (values[1] & ((uint32_t)1 << 22)) != 0;
        info->CLFLUSHOPT        = (values[1] & ((uint32_t)1 << 23)) != 0;
        info->CLWB              = (values[1] & ((uint32_t)1 << 24)) != 0;
        info->AVX512PF          = (values[1] & ((uint32_t)1 << 26)) != 0;
        info->AVX512ER          = (values[1] & ((uint32_t)1 << 27)) != 0;
        info->AVX512CD          = (values[1] & ((uint32_t)1 << 28)) != 0;
        info->SHA               = (values[1] & ((uint32_t)1 << 29)) != 0;
        info->AVX512BW          = (values[1] & ((uint32_t)1 << 30)) != 0;
        info->AVX512VL          = (values[1] & ((uint32_t)1 << 31)) != 0;
        info->PREFETCHWT1       = (values[2] & ((uint32_t)1 <<  0)) != 0;
        info->AVX512VBMI        = (values[2] & ((uint32_t)1 <<  1)) != 0;
        info->AVX512VBMI2       = (values[2] & ((uint32_t)1 <<  6)) != 0;
        info->GFNI              = (values[2] & ((uint32_t)1 <<  8)) != 0;
        info->VAES              = (values[2] & ((uint32_t)1 <<  9)) != 0;
        info->VPCLMULQDQ        = (values[2] & ((uint32_t)1 << 10)) != 0;
        info->AVX512VNNI        = (values[2] & ((uint32_t)1 << 11)) != 0;
        info->AVX512BITALG      = (values[2] & ((uint32_t)1 << 12)) != 0;
        info->AVX512VPOPCNTDQ   = (values[2] & ((uint32_t)1 << 14)) != 0;
        info->AVX5124VNNIW      = (values[3] & ((uint32_t)1 << 11)) != 0;
        info->AVX5124FMAPS      = (values[3] & ((uint32_t)1 << 11)) != 0;
        info->FSRM              = (values[3] & ((uint32_t)1 << 11)) != 0;
    }

    if (__get_cpuid(0x80000001, &values[0], &values[1], &values[2], &values[3])){
        info->ABM     = (values[2] & ((uint32_t)1 <<  5)) != 0;
        info->SSE4a   = (values[2] & ((uint32_t)1 <<  6)) != 0;
        info->XOP     = (values[2] & ((uint32_t)1 << 11)) != 0;
        info->SKINIT  = (values[2] & ((uint32_t)1 << 12)) != 0;
        info->FMA4    = (values[2] & ((uint32_t)1 << 16)) != 0;
        info->TBM     = (values[2] & ((uint32_t)1 << 21)) != 0;
        info->SYSCALL = (values[3] & ((uint32_t)1 << 11)) != 0;
        info->CMOV    = (values[3] & ((uint32_t)1 << 15)) != 0;
        info->AMD_3DNOW   = (values[3] & ((uint32_t)1 << 22)) != 0;
        info->FXSR    = (values[3] & ((uint32_t)1 << 24)) != 0;
        info->RDTSCP  = (values[3] & ((uint32_t)1 << 27)) != 0;
        info->X64     = (values[3] & ((uint32_t)1 << 29)) != 0;
    }
}
