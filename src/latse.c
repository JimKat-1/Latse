#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "util.c"

#define TYPEMASK_CONS 0x8000000000000000
// #define LATSE_C_VALUE 0x

// v's initial bits describe the type of the value and can contain the entire
// value. Whatever does not fit goes into rest, whose size should be described
// entirely by v. Most significant bit is whether this is cons or not. The next
// 7 bits are for the most usual types
typedef struct {
    uintptr_t v;
    byte rest[];
} Value;

typedef struct {
    qword name[];
} ValueSpace;
iiiiiiiiiiiiiii
enum Reprs {
    I8  = 0,
    I16 = 1,
    I32 = 2,
    I64 = 3,
    U8  = 4,
    U16 = 5,
    U32 = 6,
    U64 = 7,
    F32 = 8,
    F64 = 9,
}

int main(int argc, char **argv) {
    // for (int i = 1; i<argc; i++) {
    //     printf("%s\n", argv[i]);
    // }
}
