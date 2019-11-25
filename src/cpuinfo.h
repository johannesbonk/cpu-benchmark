// MIT License
//
// Copyright (c) 2019 Johannes Bonk and Maximilian Ley
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is

#ifndef CPUINFO
#define CPUINFO
#include <stdbool.h>

struct cpu_info {
    uint8_t BIT_VERSION; //still missing
    uint8_t CORES_PHYSICAL; //still missing
    uint8_t CORES_LOGICAL; //still missing
    bool HYP_THR;
    bool X64;
    bool FPU;
    bool PCMULQDQ;
    bool CX8;
    bool CX16;
    bool MOVBE;
    bool POPCNT;
    bool FCMOV;
    bool ERMS;
    bool INVPCID;
    bool RDSEED;
    bool PCOMMIT;
    bool CLFLUSHOPT;
    bool CLWB;
    bool SYSCALL;
    bool SKINIT;
    bool CMOV;
    bool SMX;
    bool FMA3;
    bool FMA4;
    bool AES;
    bool XSAVE;
    bool F16;
    bool RDRAND;
    bool FXSR;
    bool ABM;
    bool BMI1;
    bool BMI2;
    bool CLMUL;
    bool MMX;
    bool AVX;
    bool AVX2;
    bool AVX512F;
    bool AVX512VL;
    bool AVX512BW;
    bool AVX512CD;
    bool AVX512DQ;
    bool AVX512ER;
    bool AVX512PF;
    bool AVX512VNNI;
    bool AVX512VBMI;
    bool AVX512IFMA;
    bool AVX512VBMI2;
    bool AVX5124FMAPS;
    bool AVX512BITALG;
    bool AVX5124VNNIW;
    bool AVX512VPOPCNTDQ;
    bool SSE;
    bool SSE2;
    bool SSE3;
    bool SSSE3;
    bool SSE41;
    bool SSE42;
    bool SSE4a;
    bool SGX;
    bool TSX;
    bool INTEL_ADX;
    bool INTEL_MPX;
    bool SHA;
    bool PREFETCHWT1;
    bool GFNI;
    bool VAES;
    bool VPCLMULQDQ;
    bool FSRM;
    bool XOP;
    bool TBM;
    bool AMD_3DNOW;
    bool RDTSCP;
};

//struct takes average time after MEASUREMENT_ITERATIONS
struct execution_time {
    double X64;
    double FPU;
    double PCMULQDQ;
    double CX8;
    double CX16;
    double MOVBE;
    double POPCNT;
    double FCMOV;
    double ERMS;
    double INVPCID;
    double RDSEED;
    double PCOMMIT;
    double CLFLUSHOPT;
    double CLWB;
    double SYSCALL;
    double SKINIT;
    double CMOV;
    double SMX;
    double FMA3;
    double FMA4;
    double AES;
    double XSAVE;
    double F16;
    double RDRAND;
    double FXSR;
    double ABM;
    double BMI1;
    double BMI2;
    double CLMUL;
    double MMX;
    double AVX;
    double AVX2;
    double AVX512F;
    double AVX512VL;
    double AVX512BW;
    double AVX512CD;
    double AVX512DQ;
    double AVX512ER;
    double AVX512PF;
    double AVX512VNNI;
    double AVX512VBMI;
    double AVX512IFMA;
    double AVX512VBMI2;
    double AVX5124FMAPS;
    double AVX512BITALG;
    double AVX5124VNNIW;
    double AVX512VPOPCNTDQ;
    double SSE;
    double SSE2;
    double SSE3;
    double SSSE3;
    double SSE41;
    double SSE42;
    double SSE4a;
    double SGX;
    double TSX;
    double INTEL_ADX;
    double INTEL_MPX;
    double SHA;
    double PREFETCHWT1;
    double GFNI;
    double VAES;
    double VPCLMULQDQ;
    double FSRM;
    double XOP;
    double TBM;
    double AMD_3DNOW;
    double RDTSCP;
};

void set_cpu_info(struct cpu_info *info);

#endif
