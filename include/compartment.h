#ifndef _COMPARTMENT_H
#define _COMPARTMENT_H

#include <assert.h>
#include <elf.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>

#define align_down(x, align)    __builtin_align_down(x, align)
#define align_up(x, align)      __builtin_align_up(x, align)

#define SEG_MAX_COUNT 20

struct SegmentMap
{
    uintptr_t mem_bot;
    uintptr_t mem_top;
    size_t offset;
    size_t size;
    int prot_flags;
};

struct Compartment
{
    // Identifiers
    size_t id;
    int fd;
    // ELF data
    size_t size;
    uintptr_t entry_point;
    bool mapped;
    // Segments data
    struct SegmentMap* segs[SEG_MAX_COUNT]; // TODO
    size_t seg_count;
    // Hardware info - maybe move
    size_t page_size;
};

struct Compartment* comp_init();
struct Compartment* comp_from_elf(char*);
void comp_map(struct Compartment*);
void comp_clean(struct Compartment*);

void comp_print(struct Compartment*);
void segmap_print(struct SegmentMap*);

#endif // _COMPARTMENT_H
