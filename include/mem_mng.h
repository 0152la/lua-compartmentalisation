#ifndef MEM_MNG_H
#define MEM_MNG_H

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/mman.h>

#include <cheriintrin.h>

#define COMP_MEM_DDC_OFFSET -16
#define COMP_BIG_DDC_OFFSET -32
#define COMP_STK_OFFSET     -48

struct mem_alloc
{
    uintptr_t ptr;
    size_t size;

    struct mem_alloc* prev_alloc;
    struct mem_alloc* next_alloc;
};

extern size_t comp_mem_alloc;
extern size_t comp_mem_max;

void* my_realloc(void*, size_t);
void* my_malloc(size_t);
void my_free(void* ptr);

//void manager_register_mem_alloc(void*, size_t);
//void manager_insert_new_alloc(struct Compartment*, struct mem_alloc*);
//size_t manager_free_mem_alloc(void*);

#endif // MEM_MNG_H