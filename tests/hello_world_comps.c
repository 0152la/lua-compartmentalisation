#include "compartment.h"

int
main()
{
    char* file = "./hwelf";
    struct Compartment* hw_comp = comp_from_elf(file);
    printf("ENTRY POINT: %Pu\n", hw_comp->entry_point);
    printf("ENTRY POINT: %#010x\n", (unsigned int) hw_comp->entry_point);
    comp_print(hw_comp);
    comp_map(hw_comp);
    comp_clean(hw_comp);
    return 0;
}
