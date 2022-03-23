#include "compartment.h"

static size_t comps_id = 0;

struct Compartment*
comp_init()
{
    struct Compartment* new_comp = (struct Compartment*) malloc(sizeof(struct Compartment*));
    new_comp->id = comps_id++;
    new_comp->size = 0;
    new_comp->mapped = false;
    new_comp->seg_count = 0;
    new_comp->page_size = sysconf(_SC_PAGESIZE);
    return new_comp;
}

struct Compartment*
comp_from_elf(char* filename)
{
    struct Compartment* new_comp = comp_init();

    // Read elf headers
    Elf64_Ehdr comp_ehdr;
    new_comp->fd = open(filename, O_RDONLY);
    assert(new_comp->fd != -1);
    pread((int) new_comp->fd, &comp_ehdr, sizeof(comp_ehdr), 0);

    // Read program headers
    Elf64_Phdr comp_phdr;
    for (size_t i = 0; i < comp_ehdr.e_phnum; ++i)
    {
        pread((int) new_comp->fd, &comp_phdr, sizeof(comp_phdr),
                comp_ehdr.e_phoff + i * sizeof(comp_phdr));

        if (comp_phdr.p_type != PT_LOAD)
        {
            continue;
        }

        struct SegmentMap* this_seg =
            (struct SegmentMap*) malloc(sizeof(struct SegmentMap));
        this_seg->mem_bot = align_down(comp_phdr.p_vaddr, new_comp->page_size);
        this_seg->mem_top = comp_phdr.p_vaddr + comp_phdr.p_memsz;
        this_seg->offset = align_down(comp_phdr.p_offset, new_comp->page_size);
        this_seg->size = comp_phdr.p_filesz + (comp_phdr.p_offset & (new_comp->page_size - 1)); // TODO ????
        this_seg->prot_flags = (comp_phdr.p_flags & PF_R ? PROT_READ : 0) |
                                (comp_phdr.p_flags & PF_W ? PROT_WRITE : 0) |
                                (comp_phdr.p_flags & PF_X ? PROT_EXEC : 0);
        printf("Added segment for header %lu:\n", i);
        segmap_print(this_seg);

        new_comp->segs[new_comp->seg_count] = this_seg;
        new_comp->seg_count += 1;
        new_comp->size += this_seg->size;
        new_comp->entry_point = comp_ehdr.e_entry;
    }

    comp_print(new_comp);
    return new_comp;
}

void
comp_map(struct Compartment* to_map)
{
    struct SegmentMap* curr_seg;
    for (size_t i = 0; i < to_map->seg_count; ++i)
    {
        curr_seg = to_map->segs[i];
        printf("Currently mapping segment %lu:\n", i);
        segmap_print(curr_seg);
        void* map_result = mmap((void*) curr_seg->mem_bot,
                                curr_seg->size,
                                curr_seg->prot_flags,
                                MAP_PRIVATE | MAP_FIXED,
                                to_map->fd, curr_seg->offset);
        assert(map_result != MAP_FAILED);
    }
}

void
comp_clean(struct Compartment* to_clean)
{
    close(to_clean->fd);
    for (size_t i = 0; i < to_clean->seg_count; ++i)
    {
        free(to_clean->segs[i]);
    }
    free(to_clean);
    // TODO
}

void
comp_print(struct Compartment* to_print)
{
    printf("=== COMPARTMENT ===\n");
    printf("\t * ID    --- %lu\n", to_print->id);
    printf("\t * FD    --- %d\n", to_print->fd);
    printf("\t * SIZE  --- %lu\n", to_print->size);
    printf("\t * ENTRY --- %#010x\n", (unsigned int) to_print->entry_point);
    printf("\t * MAPD  --- %d\n", to_print->mapped);
    printf("\t * SEGC  --- %lu\n", to_print->seg_count);
    printf("\t * SEGS  --- ");
    for (size_t i = 0; i < to_print->seg_count; ++i)
    {
        printf("%p, ", to_print->segs[i]);
    }
    printf("\n");
    printf("\t * PGSZ  --- %lu\n", to_print->page_size);
}

void
segmap_print(struct SegmentMap* to_print)
{
    printf("=== SEGMENT MAP ===\n");
    printf("\t * BOT  --- %#010x\n", (unsigned int) to_print->mem_bot);
    printf("\t * TOP  --- %#010x\n", (unsigned int) to_print->mem_top);
    printf("\t * OFF  --- %zu\n", to_print->offset);
    printf("\t * SIZE --- %zu\n", to_print->size);
    printf("\t * FLAG --- %d\n", to_print->prot_flags);
}
