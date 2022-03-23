# Memory management library
add_library(maimem STATIC
    producer_consumer_mem.c
    )

# Executable - producer_consumer_test
add_executable(test
    producer_consumer.c
    producer_consumer_mem.c
    )

target_link_directories(test PUBLIC ${LUA_INSTALL_DIR}/lib)
target_link_libraries(test ${LUA_INSTALL_DIR}/lib/liblua.a dl m)
target_include_directories(test PUBLIC ${LUA_INSTALL_DIR}/include)
add_dependencies(test lua)

# Executable - hello_world_elf
add_executable(hwelf
    ${TEST_DIR}/hello_world_elf.c
    )
#target_compile_options(hwelf PUBLIC -fPIC)

add_executable(hwwrap
    ${TEST_DIR}/hello_world_comps.c
    ${SRC_DIR}/compartment.c
    )
target_include_directories(hwwrap PUBLIC ${INCLUDE_DIR})
