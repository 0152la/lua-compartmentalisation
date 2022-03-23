#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <cheriintrin.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

void print_full_cap(uintcap_t cap) {
    uint32_t words[4];  // Hack to demonstrate! In real code, be more careful about sizes, etc.
    printf("0x%d", cheri_tag_get(cap) ? 1 : 0);
    memcpy(words, &cap, sizeof(cap));
    for (int i = 3; i >= 0; i--) {
        printf("_%08x", words[i]);
    }
    printf("\n");
}

int
main(void)
{
    void* __capability new_ddc = (void* __capability) malloc(2000);
    new_ddc = cheri_bounds_set(new_ddc, 2000);
    print_full_cap((uintcap_t) new_ddc);
    void* __capability old_ddc;
    asm("mrs %w0, DDC\n\t"
        "msr DDC, %w1"
        : "+r"(old_ddc)
        : "r"(new_ddc));

    void* __capability check_ddc = cheri_ddc_get();
    assert(cheri_address_get(check_ddc) == cheri_address_get(new_ddc));

    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    lua_getglobal(L, "print");
    lua_pushstring(L, "Hello welt!");

    lua_call(L, 1, 0);

    lua_close(L);

    asm("msr DDC, %w0" : /**/ : "r"(old_ddc));

    return EXIT_SUCCESS;
}
