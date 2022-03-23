#include <cheriintrin.h>

#include <stdio.h>
#include <stdlib.h>

#if defined(__CHERI_PURE_CAPABILITY__) ||!__has_feature(capabilities)
#  error This example must be run on a CHERI hybrid system
#endif

void ddc_set(void *__capability cap) {
    asm volatile("MSR DDC, %[cap]" : : [cap] "C"(cap) : "memory");
}

int main() {
    void *__capability old_ddc = cheri_ddc_get();
    void *__capability new_ddc = cheri_bounds_set(cheri_ddc_get(), 0x0900000000000);
    printf("%#lp\n", cheri_ddc_get());
    printf("%#lp\n", new_ddc);
    ddc_set(new_ddc);
    ddc_set(old_ddc);
    return 0;
}
