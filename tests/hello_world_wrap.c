#include "hello_world_elf.h"

#define ROUNDUP(x, y)   ((((x)+((y)-1))/(y))*(y))
#define ALIGNDOWN(k, v) ((unsigned long)(k)&(~((unsigned long)(v)-1)))

void
do_print_fn()
{
    printf("My string is hallo welt.\n");
}

void *memcopy(void *dest, const void *src, unsigned long n) {
    unsigned long i;
    unsigned char *d = (unsigned char *)dest;
    unsigned char *s = (unsigned char *)src;

    for (i = 0; i < n; ++i)
        d[i] = s[i];

    return dest;
}

void *
get_elf_func(const char* fn_name)
{
    void* this_hdl = dlopen(NULL, RTLD_NOW | RTLD_DEEPBIND);
    if (!this_hdl)
    {
        printf("Error loading `dlopen`: %s.\n", dlerror());
        exit(1);
    }

    void* find_sym = dlsym(this_hdl, fn_name);
    if (!find_sym)
    {
        printf("Error finding symbol `%s`: %s\n.", fn_name, dlerror());
        exit(1);
    }
    return find_sym;
}

void
call_elf_func(void* fn)
{
    /*void (*fn_c)() = (void (*)()) (fn);*/
    /*fn_c();*/
    asm("str lr, [sp, #-16]!\n\t"
        "blr %[fn]\n\t"
        "ldr lr, [sp], #16"
         :
         : [fn]"r"(fn)
         : "memory");
}

int
main()
{
    /*void* f = get_elf_func("do_print");*/
    /*call_elf_func(f);*/

    // Open binary file which we want to compartmentalize
    char* comp_file = malloc(256);
    getcwd(comp_file, 256);
    strcat(comp_file, "/hwelf");

    // Get size of binary (and therefore compartment)
    size_t comp_file_size;
    FILE* comp_fd = fopen(comp_file, "r");
    fseek(comp_fd, 0, SEEK_END);
    comp_file_size = ftell(comp_fd);
    rewind(comp_fd);
    printf("File size is `%lu`.\n", comp_file_size);

    // Copy ELF data in memory
    void* comp_addr = mmap(NULL, comp_file_size,
                           PROT_READ | PROT_WRITE,
                           MAP_PRIVATE | MAP_ANONYMOUS,
                           -1, 0);
    const size_t step_bytes_count = 1024;
    char* buf[step_bytes_count];
    fread(comp_addr, comp_file_size, 1, comp_fd);
    fclose(comp_fd);

    // Set permissions?
	Elf64_Ehdr* comp_ehdr = (Elf64_Ehdr *) comp_addr;
    Elf64_Phdr* comp_phdr = (Elf64_Phdr *) (comp_addr + comp_ehdr->e_phoff);
    assert(comp_phdr->p_vaddr != 0x0);

    // Find section containing `.text` (therefore `entry_point`)
    void* comp_entry = (void*) comp_ehdr->e_entry;
    /*Elf64_Shdr* comp_shdr = (Elf64_Shdr *) (comp_addr + comp_ehdr->e_shoff);*/
    /*while (*/

    unsigned long comp_base_vaddr = 0;
    void* text_segment = NULL;
    unsigned long break_addr = 0;
    for (size_t i = 0; i < comp_ehdr->e_phnum; ++i, ++comp_phdr)
    {
        // Skip if current segment is not loadable
        if (comp_phdr->p_type != PT_LOAD)
        {
            continue;
        }

        if (comp_phdr->p_type == PT_INTERP)
        {
            assert(0 && "Unimplemented");
        }

        unsigned long addr_to_map_curr_segment_at = ALIGNDOWN(comp_phdr->p_vaddr, 0x1000);
        unsigned long segment_len = (unsigned long) comp_phdr->p_memsz;
        segment_len += (unsigned long) comp_phdr->p_vaddr % 0x1000; // allignment
        segment_len = ROUNDUP(segment_len, 0x1000);

        /*if (text_segment != 0)*/
        /*{*/
            /*unsigned long unaligned_map_addr =*/
                /*(unsigned long) text_segment +*/
                /*(unsigned long) comp_phdr->p_vaddr -*/
                /*(unsigned long) comp_base_vaddr;*/
            /*addr_to_map_curr_segment_at = ALIGNDOWN(unaligned_map_addr, 0x1000);*/
        /*}*/
        /*else*/
        /*{*/
            /*addr_to_map_curr_segment_at = 0UL;*/
        /*}*/

        /*if (comp_base_vaddr == 0)*/
        /*{*/
            /*comp_base_vaddr = comp_phdr->p_vaddr;*/
        /*}*/


        // Allocate mem for current segment and copy it over
        void* mapped_segment =
            mmap((void*) addr_to_map_curr_segment_at, segment_len,
            PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        assert(mapped_segment != MAP_FAILED && "Failed to map new segment");
        /*memcopy(mapped_segment,*/
        memcopy((void*) ((unsigned long) mapped_segment + ((unsigned long) comp_phdr->p_vaddr % 0x1000)),
            comp_addr + comp_phdr->p_offset,
            comp_phdr->p_filesz);

        /*if (comp_phdr->p_vaddr < comp_entry && comp_entry < comp_phdr->p_vaddr + comp_phdr->p_memsz)*/
        /*{*/
            /*text_segment = mapped_segment;*/
            /*comp_base_vaddr = comp_phdr->p_vaddr;*/
            /*if (anywhere)*/
            /*comp_entry = (void*) (*/
                /*(uintptr_t) comp_ehdr->e_entry -*/
                /*(uintptr_t) comp_phdr->p_vaddr +*/
                /*(uintptr_t) comp_addr);*/
        /*}*/

        unsigned int segment_flags = (
            ((comp_phdr->p_flags & PF_R) ? PROT_READ  : 0) |
            ((comp_phdr->p_flags & PF_W) ? PROT_WRITE : 0) |
            ((comp_phdr->p_flags & PF_X) ? PROT_EXEC  : 0) );
        mprotect(mapped_segment, segment_len, segment_flags);

        unsigned long curr_upper_bound = comp_phdr->p_vaddr + comp_phdr->p_memsz;
        if (curr_upper_bound > break_addr)
        {
            break_addr = curr_upper_bound;
            // TODO?
        }

    }
    munmap(comp_addr, comp_file_size);

    // Jump to compartment entry point
    call_elf_func(comp_entry);

    // Exit
    printf("Finalizing wrapper execution.");
}
