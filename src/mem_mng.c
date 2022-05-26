#include "mem_mng.h"

size_t comp_mem_alloc = 0;
size_t comp_mem_max = 0x4000UL;

void*
my_realloc(void* ptr, size_t to_alloc)
{
    if (cheri_ddc_get() == NULL)
    {
        return realloc(ptr, to_alloc);
    }

    if (ptr == NULL)
    {
        return my_malloc(to_alloc); // TODO
    }

    assert(0);

    /*uintptr_t ddc_addr = cheri_address_get(cheri_ddc_get());*/
    /*size_t comp_size = cheri_length_get(cheri_ddc_get());*/
    /*int* alloc_mem_addr = (int*) (ddc_addr + comp_size + COMP_MEM_DDC_OFFSET);*/
    /*size_t alloc_mem = *alloc_mem_addr;*/

    /*assert(alloc_mem + to_alloc <= comp_size);*/
    /*void* new_mem = mmap((void*) (ddc_addr + alloc_mem),*/
                         /*to_alloc,*/
                         /*PROT_READ | PROT_WRITE,*/
                         /*MAP_PRIVATE | MAP_ANONYMOUS,*/
                         /*-1, 0);*/

    /**alloc_mem_addr = alloc_mem + to_alloc;*/
    /*void* new_mem = (void*) (ddc_addr + alloc_mem);*/
    /*assert(new_mem != MAP_FAILED);*/

    /*return new_mem;*/
}

void*
my_malloc(size_t to_alloc)
{
    uintptr_t comp_base_addr = cheri_address_get(cheri_ddc_get());
    /*assert(comp_mem_alloc + to_alloc < comp_mem_max);*/
    /*assert(comp->scratch_mem_alloc + to_alloc < comp->scratch_mem_size);*/

    /*void* new_mem = mmap((void*) (comp->scratch_mem_base + comp->scratch_mem_alloc),*/
    void* new_mem = mmap((void*) (comp_base_addr + comp_mem_alloc),
                         to_alloc,
                         PROT_READ | PROT_WRITE,
                         MAP_PRIVATE | MAP_ANONYMOUS,
                         -1, 0);
    /*comp->scratch_mem_alloc += to_alloc;*/
    comp_mem_alloc += to_alloc;
    /*manager_register_mem_alloc(new_mem, to_alloc); // TODO*/
    return new_mem;
}

void
my_free(void* ptr)
{
    return;
    /*size_t ptr_size = manager_free_mem_alloc(ptr); // TODO*/
    /*if (munmap(ptr, ptr_size) != 0)*/
    /*{*/
        /*assert(0); // TODO*/
    /*}*/
    /*return;*/
}

// MEM TODO

/*void*/
/*manager_register_mem_alloc(void* ptr, size_t mem_size)*/
/*{*/
    /*struct Compartment* comp = manager_find_compartment_by_addr(ptr);*/

    /*struct mem_alloc* new_alloc = malloc(sizeof(struct mem_alloc));*/
    /*new_alloc->ptr = (uintptr_t) ptr;*/
    /*new_alloc->size = mem_size;*/
    /*manager_insert_new_alloc(comp, new_alloc);*/
/*}*/

/*void*/
/*manager_insert_new_alloc(struct Compartment* comp, struct mem_alloc* to_insert)*/
/*{*/
    /*if (comp->alloc_head == NULL)*/
    /*{*/
        /*to_insert->prev_alloc = NULL;*/
        /*to_insert->next_alloc = NULL;*/
        /*comp->alloc_head = to_insert;*/
        /*return;*/
    /*}*/

    /*if (comp->alloc_head->ptr > to_insert->ptr)*/
    /*{*/
        /*to_insert->next_alloc = comp->alloc_head;*/
        /*comp->alloc_head->prev_alloc = to_insert;*/
        /*comp->alloc_head = to_insert;*/
        /*return;*/
    /*}*/

    /*struct mem_alloc* curr_alloc = comp->alloc_head;*/
    /*while(curr_alloc->next_alloc != NULL && curr_alloc->ptr < to_insert->ptr)*/
    /*{*/
        /*curr_alloc = curr_alloc->next_alloc;*/
    /*}*/
    /*if (curr_alloc->next_alloc == NULL)*/
    /*{*/
        /*to_insert->prev_alloc = curr_alloc;*/
        /*curr_alloc->next_alloc = to_insert;*/
        /*to_insert->next_alloc = NULL;*/
        /*return;*/
    /*}*/

    /*to_insert->next_alloc = curr_alloc->next_alloc;*/
    /*to_insert->next_alloc->prev_alloc = to_insert;*/
    /*curr_alloc->next_alloc = to_insert;*/
    /*to_insert->prev_alloc = curr_alloc;*/
    /*return;*/
/*}*/

/*size_t*/
/*manager_free_mem_alloc(void* ptr)*/
/*{*/
    /*struct Compartment* comp = manager_find_compartment_by_addr(ptr);*/
    /*struct mem_alloc* curr_alloc = comp->alloc_head;*/
    /*while (curr_alloc != NULL && curr_alloc->ptr != (uintptr_t) ptr)*/
    /*{*/
        /*curr_alloc = curr_alloc->next_alloc;*/
    /*}*/

    /*assert(curr_alloc != NULL);*/
    /*if (curr_alloc->prev_alloc != NULL)*/
    /*{*/
        /*curr_alloc->prev_alloc->next_alloc = curr_alloc->next_alloc;*/
    /*}*/
    /*if (curr_alloc->next_alloc != NULL)*/
    /*{*/
        /*curr_alloc->next_alloc->prev_alloc = curr_alloc->prev_alloc;*/
    /*}*/
    /*size_t to_return = curr_alloc->size;*/
    /*free(curr_alloc);*/

    /*return to_return;*/
/*}*/
