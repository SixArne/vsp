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








# Fetch glslang repository
FetchContent_Declare(
        glslang
        GIT_REPOSITORY https://github.com/KhronosGroup/glslang
        GIT_TAG "main"
        DOWNLOAD_NO_PROGRESS ON
)

FetchContent_GetProperties(glslang)
if(NOT glslang_POPULATED)
    FetchContent_Populate(glslang)
endif()

# Create a custom target that depends on the glslang target
add_custom_target(run_python_command
        DEPENDS ${glslang_POPULATED_NAME}
)

# Add a custom command to execute a Python command
add_custom_command(
        TARGET run_python_command
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E chdir
        ${glslang_SOURCE_DIR}
        ${CMAKE_COMMAND} -E echo "Running your Python command in ${glslang_SOURCE_DIR}"
        COMMAND ${CMAKE_COMMAND} -E python YourPythonScript.py  # Replace with your Python script
        COMMENT "Executing Python command after populating glslang"
)

FetchContent_MakeAvailable(glslang)

find_package(Vulkan REQUIRED)

# Adding include directories
target_include_directories(vsp PUBLIC
        Vulkan::Headers
        ${glsl_INCLUDE_DIR}
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

# Export vsp_INCLUDE_DIR to parent scope
set(vsp_INCLUDE_DIR "${vsp_INCLUDE_DIR}" CACHE INTERNAL "vsp include directory")