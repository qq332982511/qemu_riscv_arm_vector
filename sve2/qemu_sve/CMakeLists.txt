cmake_minimum_required(VERSION 3.15.2)
project(qemu_a53 LANGUAGES C ASM)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

set(MEGCC_RUNTIME_DIR "${CMAKE_SOURCE_DIR}/../../")
if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

file(GLOB TEST_SRC src/*.c src/*.s)
file(GLOB QEMU_LD_FILE src/link.ld)
add_link_options(-Wl,-T,${QEMU_LD_FILE} -lm )
add_executable(test_elf ${TEST_SRC} )
target_link_libraries(test_elf PRIVATE "-lm")
target_include_directories(test_elf PRIVATE ${CMAKE_SOURCE_DIR}/../)
