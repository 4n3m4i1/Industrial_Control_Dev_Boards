cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(WAFFLE_MINI_PROJECT C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_EXAMPLES_PATH ${PROJECT_SOURCE_DIR})

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(
                ${PROJECT_NAME}
                main.c
                )

# Link Libraries
target_link_libraries(
                        ${PROJECT_NAME}
                        pico_stdlib
                        )

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

# Create EXEC binaries like uf2
pico_add_extra_outputs(${PROJECT_NAME})