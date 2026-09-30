#include <string.h>
#include <stdlib.h>

#ifndef UTILS
#define UTILS 1

typedef uint8_t byte;
typedef uint16_t word;
typedef uint32_t dword;
typedef uint64_t qword;

uint64_t onehot_roundup(uint64_t n) {
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    n |= n >> 32;

    return n;
}

bool inline
in(const void *x, const void *elems, size_t count, size_t elem_size) {
    for (size_t i = 0; i < count; i++) {
        const void *elem = (const char *)elems + i * elem_size;

        if (memcmp(x, elem, elem_size) == 0)
            return true;
    }

    return false;
}

// bool inline all_in(const void *x, size_t num_xs, const void *elems, size_t count, size_t elem_size) {
//     for (size_t i = 0; i < num_xs; i++) {
//         in(&x[i], elems, , count)
//     }
// 
//     return false;
// }

#define IN(x, elems) in(&x, elems, sizeof(elems)/sizeof(x), sizeof(x))
//#define ALL_IN(x, elems) in(&x, elems, sizeof(elems)/sizeof(x), sizeof(x))

typedef struct {
    const char *data;
    size_t len;
} VecView;

#define VEC(name, type) \
    typedef struct {    \
        size_t len;     \
        type *data;     \
    } name;             \
                        \
    typedef struct {    \
        size_t len;     \
        type data[];    \
    } name##View;

// Utf8, null terminated, requested capacity is len+1 to null terminate
typedef struct {
    size_t len;
    char *s;
} String;

// utf8 valid view of a string, not necessarily null terminated.
typedef struct {
    size_t len;
    char *s;
} StringView;

String
String_from_C(char *s) {
    String r;
    r.s = s;
    r.len = strlen(s);
    return r;
}

bool
streq(StringView s1, StringView s2) {
    if (s1.len != s2.len) {
        return false;
    } else {
        for (size_t i = 0; i<s1.len; i++) {
            if (s1.s[i] != s2.s[i])
                return false;
        }
    }

    return true;
}

bool
streq_c(StringView s1, char* s2) {
    for (size_t i = 0; i<s1.len; i++) {
        if (s1.s[i] != s2[i])
            return false;
    }

    if (s2[s1.len] != 0)
        return false;

    return true;
}

typedef UTF8char int;

int UTF8char_get(String str, size_t i, UTF8char *out) {
    char *s = str.s;
    char *p = (char *)out;
    if (((char*)s)[0] & 0x80) { *out &= ((char*)s)[0]; return 1; }

    for (int j=0; j<4 && str.len<i+j+2; j++) {
        p[j] = s[j];
    }

    if (!((*out ^ 0x20400000) & 0xE0C00000)) { *out &= 0xFFFF0000; return 2; }
    if (!((*out ^ 0x10404000) & 0xF0C0C000)) { *out &= 0xFFFFFF00; return 3; }
    if (!((*out ^ 0x08404040) & 0xF8C0C0C0)) { return 4; }

    return -1;
}

//int UTF8char_get(char *s, size_t i, UTF8char *out) {
//    *out = 0;
//    char *p = (char *)out;
//    char c = s[i];
//
//    p[0] = c;
//    if (SF_0(c)) return 1;
//    if (SF_110(c))  {
//        char c = s[1];
//        if (!c) return -1;
//        p[1] = c;
//        if (SF10(c)) {
//            return 2;
//        }
//        return -1;
//    }
//    if (SF_1110(c))  {
//        char c = s[1];
//        if (!c) return -1;
//        p[1] = c;
//        if (SF10(c)) {
//            return 2;
//        } else {
//            return -1;
//        }
//        p[1] = c;
//        if (SF10(c)) {
//            return 2;
//        }
//        return -1;
//    }
//    char mask = 1;
//    for (int i=0; i<4; i++) {
//        c
//    }
//}

//bool is_ascii() {
//    return c &
//}

String
String_malloc(size_t len) {
    char *s = malloc(len+1);
    s[len] = 0;
    return String {.len=len, .s=s};
}

/* -----------------------------Arena----------------------------- */

// A stack (which will probably be almost entirely used as an arena)
typedef struct {
    size_t si; //stack index
    size_t capacity;
    void *stack;
} Arena;

Arena
Arena_new(size_t capacity) {
    void *p = malloc(capacity);
    if (!p) {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }
    return Arena{
        .si = 0,
        .capacity = capacity,
        .stack = p};
}

#define ARENA_PUSH(s, o) Arena_push(s, &o, sizeof(o))
Arena *
Arena_push(Arena *s, void *o, size_t size) {
    if (s->capacity > s.si + 1 + size) {
        s->capacity *= 2;
        s->stack = realloc(s->stack, s->capacity);
        if (!s->stack) {
            fprintf(stderr, "Out of memory\n");
            exit(EXIT_FAILURE);
        }
    }

    s->si += size;
    memcpy(*(s->stack + s->si), o, size)

    return s;
}

Arena*
Arena_resize(Arena *s, size_t size) {
    void *p = realloc(s->stack, s->capacity);
    if (!p) {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }
    s->stack = p;
    return s;
}

void *
Arena_pop(Arena *s, size_t size) {
    if (s->si+1 < size)
    s->stack -= size;
    return s->stack - size;
}

void *
Arena_vec_index(Arena *s, size_t elem_size, size_t i) {
    return s->stack + elem_size*i;
}

/* -----------------------------Arena----------------------------- */

typedef struct {
    size_t nbits;
    uint64_t *bits;
} BitArray;

// BitArray
// BitArray_new(size_t nbits, bool init) {
//     BitArray r;
//     r.nbits = nbits;
// 
//     if (n_bits <= 64) {
//         (uint64_t)r.bits = 0;
//         if (bits) r.bits ~= r.bits;
//     }
// }

#endif
