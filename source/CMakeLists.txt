set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# Defining library files
add_library(vsp STATIC
    vsp/vsp.h
    vsp/vsp.cpp
)

# Adding include directories
target_include_directories(vsp PUBLIC
    Vulkan::Headers
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Adding dependencies
target_link_libraries(vsp PUBLIC
    glslang
    SPIRV
    OGLCompiler
    Vulkan::Vulkan
)

# Adding include directories for library users
set(vsp_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vsp")


################################################################################################
# Extra configuration
################################################################################################

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Debug mode enabled. Optimizations disabled.")
    target_compile_definitions(vsp PUBLIC DEBUG=1)  
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Release mode enabled. Optimizations enabled.")
    target_compile_definitions(vsp PUBLIC DEBUG=0)
endif()

if(MSVC)
 target_compile_options(vsp PRIVATE "/MP")
endif()

# GLSLang
FetchContent_Declare(
    glslang
    GIT_REPOSITORY https://github.com/KhronosGroup/glslang
    GIT_TAG "main"
    DOWNLOAD_NO_PROGRESS ON
)

FetchContent_MakeAvailable(glslang)

find_package(Vulkan REQUIRED)

# Export vsp_INCLUDE_DIR to parent scope
set(vsp_INCLUDE_DIR "${vsp_INCLUDE_DIR}" CACHE INTERNAL "vsp include directory")