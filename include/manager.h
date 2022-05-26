#ifndef _MANAGER_H
#define _MANAGER_H

#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <dlfcn.h>
#include <sys/auxv.h>

// vDSO wrapper needed includes
#include <sys/time.h>
//#include <sys/vdso.h>


/*******************************************************************************
 * Memory allocation
 ******************************************************************************/

#include "mem_mng.h"

struct Compartment* manager_find_compartment_by_addr(void*);

/*******************************************************************************
 * Compartment function intercepts
 ******************************************************************************/

struct func_intercept {
    char* func_name;
    uintptr_t redirect_func;
};

// Intercept functions
time_t manager_time();

#define MAX_INTERCEPT_COUNT 4
static const struct func_intercept comp_intercept_funcs[] = {
    /* vDSO funcs */
    { "time", (uintptr_t) manager_time },
    //"printf",
    /* Mem funcs */
    { "malloc", (uintptr_t) my_malloc },
    { "realloc", (uintptr_t) my_realloc },
    { "free", (uintptr_t) my_free },
};

/*******************************************************************************
 * Compartment
 ******************************************************************************/

#include "compartment.h"

static struct Compartment* loaded_comp = NULL;

#define ENV_FIELDS_CNT 1
extern const char* comp_env_fields[ENV_FIELDS_CNT];
extern char** environ;

const char* get_env_str(const char*);
int manager___vdso_clock_gettime(clockid_t, struct timespec*);

#endif // _MANAGER_H
