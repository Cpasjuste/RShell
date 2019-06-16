cmake_minimum_required(VERSION 3.0)
#set(CMAKE_VERBOSE_MAKEFILE ON)

# delete cmake cache folder before changing this options
option(BUILD_LINUX "Build with SDL2 support" ON)
option(BUILD_3DS "Build with 3DS support" OFF)
option(BUILD_SWITCH "Build with Nintendo Switch support" OFF)

option(OPTION_THREADS "Build with threading support" OFF)

# setup toolchain
# set(CMAKE_TOOLCHAIN_FILE Toolchain.cmake)
include(Toolchain.cmake)

project(librshell)

##############
# libRShell
##############
set(SHELL_INCLUDES server/includes common)
file(GLOB SHELL_SOURCES server/sources/*.c common/*.c)

if (OPTION_THREADS)
    list(APPEND FLAGS -D__THREADS__)
endif ()

#####################
# PLATORM SPECIFIC
#####################
if (BUILD_3DS)
    #####################
    # 3DS PLATORM
    #####################
    set(PLATFORM_INCLUDES
            ${DEVKITPRO}/libctru/include
            ${DEVKITPRO}/portlibs/3ds/include)
    file(GLOB PLATFORM_SOURCES)
    set(PLATFORM_LIBS ctru)
    list(APPEND FLAGS -O3 -D__3DS__)
    #####################
    # SWITCH PLATORM
    #####################
elseif (BUILD_SWITCH)
    ########################
    # LINUX PLATORM (SDL2)
    ########################
elseif (BUILD_LINUX)
    set(PLATFORM_INCLUDES server/sources/unix)
    file(GLOB PLATFORM_SOURCES server/sources/unix/*.c)
    set(PLATFORM_LIBS)
    list(APPEND FLAGS -D__LINUX__)
endif ()

add_library(${PROJECT_NAME} ${SHELL_SOURCES} ${PLATFORM_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${SHELL_INCLUDES} ${PLATFORM_INCLUDES})
target_link_libraries(${PROJECT_NAME} PUBLIC ${PLATFORM_LIBS})
target_compile_options(${PROJECT_NAME} PUBLIC ${FLAGS} -Wno-narrowing)

#install(FILES ${CMAKE_BINARY_DIR}/libcross2d.a DESTINATION lib)
#install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/cross2d DESTINATION include)

#####################
# test executable
#####################
set(PROJECT_AUTHOR "Cpasjuste")
set(VERSION_MAJOR "1")
set(VERSION_MINOR "0")
add_executable(${PROJECT_NAME}.elf test/main.c)
target_link_libraries(${PROJECT_NAME}.elf ${PROJECT_NAME})
