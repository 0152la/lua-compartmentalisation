#ifndef PRODUCER_CONSUMER_MEM_H
#define PRODUCER_CONSUMER_MEM_H

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/mman.h>

#include <cheriintrin.h>

#define COMP_MEM_DDC_OFFSET -16
#define COMP_BIG_DDC_OFFSET -32
#define COMP_STK_OFFSET     -48

void* my_realloc(size_t);

#endif // PRODUCER_CONSUMER_MEM_H
