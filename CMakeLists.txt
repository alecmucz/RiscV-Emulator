cmake_minimum_required(VERSION 3.20)
project(riscv_emulator CXX)

set(CMAKE_CXX_STANDARD 26)              #c++ 26
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)           # Ensure ISO Standard

add_library(rv_core
        src/rv/frontend/decoder.cpp
        src/rv/backend/memory.cpp
        src/rv/backend/executor.cpp
        src/rv/backend/hart.cpp
        src/rv/backend/peripherals/uart.cpp
        src/rv/loader/elf_loader.cpp
)
target_include_directories(rv_core PUBLIC include)
target_compile_options(rv_core PRIVATE -Wall -Wextra -Wpedantic)

add_executable(rv apps/main.cpp
)
target_link_libraries(rv PRIVATE rv_core)

# Catch 2 Dependency
include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.15.2
)

FetchContent_MakeAvailable(Catch2)

enable_testing()
add_executable(decoder_test tests/frontend/decoder_test.cpp)
target_link_libraries(decoder_test PRIVATE rv_core Catch2::Catch2WithMain)
add_test(NAME decoder_test COMMAND decoder_test)
