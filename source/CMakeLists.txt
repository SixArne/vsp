set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# Defining library files
add_library(vsp STATIC
    vsp/ShaderLoader.h
    vsp/ShaderLoader.cpp
    vsp/ShaderCompiler.h
    vsp/ShaderCompiler.cpp
)

# Adding include directories
target_include_directories(vsp PUBLIC
    Vulkan::Headers
    ${SPDLOG_INCLUDE_DIR}/include
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

target_compile_definitions(vsp PUBLIC SPDLOG_ACTIVE_LEVEL=0)

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

# spdlog
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/SixArne/spdlog.git
    GIT_TAG "v1.x"
    DOWNLOAD_NO_PROGRESS ON
)

FetchContent_GetProperties(spdlog)
if(NOT spdlog_POPULATED)
    FetchContent_Populate(spdlog)
    message(STATUS "Fetching spdlog from https://github.com/SixArne/spdlog.git on branch v1.x")
    message(STATUS "Setting SPDLOG_INCLUDE_DIR to ${spdlog_SOURCE_DIR}")
    set(SPDLOG_INCLUDE_DIR "${spdlog_SOURCE_DIR}")
endif()

find_package(Vulkan REQUIRED)

# Export vsp_INCLUDE_DIR to parent scope
set(vsp_INCLUDE_DIR "${vsp_INCLUDE_DIR}" CACHE INTERNAL "vsp include directory")