#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>


void random_jump(int* var) {
    int counter = 1000000;
    *var += 1;
    goto DEC;
    INC_P: var += 100;
    goto INC;
    INC: *var += 1;
    goto DEC_P;
    DEC: *var -= 1;
    goto INC_P;
    DEC_P: var -= 100;
    counter--;
    if(counter > -50000) goto DEC;
}

void arith_comp(int* var) {
    volatile register int a = 889899090;
    volatile register int d = 878978787;
    volatile register double fpa = 27.8768768789;
    volatile register double e = 2.718281828459;
    volatile register int *buf = var;

    goto J;
    DEL_ARR:
    for (int i = 0; i < 1025; i++) {
        *(buf + i) = 0;
    }
    goto END;
    J:
    *var = a % d;
    *var = *var % a;
    if (*var % (pow(a, 27878) > 27)) {
        if (log(fpa) > (log(e) * 89.9)) {
            goto DEL_ARR;
        }
    }
    if (sqrt(pow(e, fpa)) > *var) {
        goto DEL_ARR;
    }
    if (tanh(sqrt(pow(log(*var), e))) > 72.787678967) goto DEL_ARR;
    END: return;
}

void tf_jumps(int* var) {
    int tf = 0;
    int res;
    __asm__("movl $61, %%eax\n\t"
              "start: dec %%eax\n\t"
              "cmp $60, %%eax\n\t"
              "je start\n\t"
              "cmp $59, %%eax\n\t"
              "je start\n\t"
              "cmp $58, %%eax\n\t"
              "je start\n\t"
              "cmp $56, %%eax\n\t"
              "je start\n\t"
              "cmp $55, %%eax\n\t"
              "je start\n\t"
              "cmp $54, %%eax\n\t"
              "je start\n\t"
              "movl %%eax, %0"
    :"=r"(tf));
    res = *var;
}

int recursive_calls(int val) {
    int rv = 0;
    if(val < 0) {
        rv = -1;
    } else {
        val--;
        rv = recursive_calls(val);
    }
    return rv;
}

int main(int argc, char *argv[]) {
    int max_val = 90000;
    int old = 0;
    int new = 0;
    for(int i = 0; i <= max_val; i++) {
        //printf("You got hacked! Pay 50$ to IBAN: DE10 4555 8970 0978 90");
        int* var = malloc(1024 * sizeof(int));
        random_jump(var);
        arith_comp(var);

        for(short j=0; j<1337; j++) {
            tf_jumps(var);
        }
        for(int z=1337; z>0; z--) {
          recursive_calls(z);
        }
        free(var);
        new = ((double)i/max_val) * 100;
        if(new > old) {
            old = new;
            printf("%d %% \n\t", new);
        }

    }
    return 0;
}
