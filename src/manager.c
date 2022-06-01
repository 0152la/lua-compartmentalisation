#include "manager.h"

const char* comp_env_fields[] = { "PATH", };
void* __capability manager_ddc = NULL;
struct Compartment* loaded_comp = NULL; // TODO

const char*
get_env_str(const char* env_name)
{
    size_t env_name_len = strlen(env_name);
    for (char** env = environ; env != NULL; ++env) {
        const char* str = *env;
        if (strncmp(str, env_name, env_name_len) == 0 && str[env_name_len] == '=')
            return str;
    }
    return NULL;
}

time_t
manager_time(time_t* t)
{
    return time(t);
}

int
manager___vdso_clock_gettime(clockid_t clock_id, struct timespec* ts)
{
    void* vdso;
    int res =  (uintptr_t) elf_aux_info(AT_KPRELOAD, vdso, sizeof(vdso));
    assert(res == 0);
    int (*vdso_call)(clockid_t, struct timespec*) = dlsym(vdso, "__vdso_clock_gettime");

    /*int (*vdso_call)(clockid_t, struct timespec*) = __vdso_clock_gettime;;*/

    assert(vdso_call != NULL);
    return vdso_call(clock_id, ts);
}

void*
my_realloc(void* ptr, size_t to_alloc)
{
    struct Compartment* comp = manager_find_compartment_by_ddc(cheri_ddc_get());

    if (ptr == NULL)
    {
        return my_malloc(to_alloc); // TODO
    }

    manager_free_mem_alloc(comp, ptr);
    return manager_register_mem_alloc(comp, to_alloc);

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
    struct Compartment* comp = manager_find_compartment_by_ddc(cheri_ddc_get());
    assert(comp->scratch_mem_alloc + to_alloc < comp->scratch_mem_size);
    void* new_mem = manager_register_mem_alloc(comp, to_alloc);
    comp->scratch_mem_alloc += to_alloc;
    return new_mem;
}

void
my_free(void* ptr)
{
    struct Compartment* comp = manager_find_compartment_by_ddc(cheri_ddc_get());
    manager_free_mem_alloc(comp, ptr); // TODO
    return;
}

struct Compartment*
manager_find_compartment_by_addr(void* ptr)
{
    return loaded_comp; // TODO
}

struct Compartment*
manager_find_compartment_by_ddc(void* __capability ddc)
{
    return loaded_comp; // TODO
}
