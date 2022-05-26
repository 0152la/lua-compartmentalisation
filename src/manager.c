#include "manager.h"

const char* comp_env_fields[] = { "PATH", };

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

struct Compartment*
manager_find_compartment_by_addr(void* ptr)
{
    return loaded_comp; // TODO
}
