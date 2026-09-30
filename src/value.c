#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "util.c"

#define TYPEMASK_CONS 0x8000000000000000

typedef struct {
    uintptr_t v;
    byte rest[];
} Value;

typedef struct {
    qword name[];
} ValueSpace;

enum Reprs {
    I8  = 0,
    U8  = 1,
    I16 = 2,
    U16 = 3,
    I32 = 4,
    U32 = 5,
    I64 = 6,
    U64 = 7,
    F32 = 8,
    F64 = 9,
}

Value
symbol(StringView s) {}

Value
cons(Value v1, Value v2) {}
