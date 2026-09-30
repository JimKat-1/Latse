#include <sys/mman.h>
#include "util.c"

// A contiguous chunk of available pages
typedef struct {
    // First 2 bits stand for 0: 4Kib page, 1: 2Mib page, 3: 1Gib page. Rest is
    // total size of contiguous pages
    uint64_t size;
    void *page;
} PageChunk;

// A collection of page chunks
typedef struct {
    PageChunk *chunks;
} Pages;
