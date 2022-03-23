#include "producer_consumer_mem.h"

void*
my_realloc(size_t to_alloc)
{
    uintptr_t ddc_addr = cheri_address_get(cheri_ddc_get());
    size_t comp_size = cheri_length_get(cheri_ddc_get());
    int* alloc_mem_addr = (int*) (ddc_addr + comp_size + COMP_MEM_DDC_OFFSET);
    size_t alloc_mem = *alloc_mem_addr;

    assert(alloc_mem + to_alloc <= comp_size);
    /*void* new_mem = mmap((void*) (ddc_addr + alloc_mem),*/
                         /*to_alloc,*/
                         /*PROT_READ | PROT_WRITE,*/
                         /*MAP_PRIVATE | MAP_ANONYMOUS,*/
                         /*-1, 0);*/

    *alloc_mem_addr = alloc_mem + to_alloc;
    void* new_mem = (void*) (ddc_addr + alloc_mem);
    /*assert(new_mem != MAP_FAILED);*/

    return new_mem;
}

void
my_free()
{
}
