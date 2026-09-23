# ----------------------------
# How to use this build system:
# If you are inside of the root directory of this project:
#   cmake -B build
#   cmake --build build
# If you are already inside of a build directory:
#   cmake -S ..
#   cmake --build .
# ----------------------------
cmake_minimum_required(VERSION 3.10)

# The project itself 
# (sets a bunch of platform things under the hood,
#  hence why we call it first)
project(triangles)

# Policy that OpenGL asks for
cmake_policy(SET CMP0072 NEW)

# Needed packages/libraries
find_package(OpenGL REQUIRED)
find_package(glfw3 3.5 REQUIRED)
find_package(glm REQUIRED)

# Include paths
include_directories(include)

# Build flags
# To switch to debug/release mode, add
# '-DCMAKE_BUILD_TYPE=Release/Debug' to your cmake command
# This sets the build type to Debug if it hasn't been set
if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
  set(CMAKE_BUILD_TYPE Debug)
endif()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS 
    "-Wall -Wextra -Weffc++                                    \
		 -Waggressive-loop-optimizations                           \
		 -Wc++14-compat -Wmissing-declarations -Wcast-align        \
		 -Wcast-qual -Wchar-subscripts -Wconditionally-supported   \
		 -Wconversion -Wctor-dtor-privacy -Wempty-body             \
		 -Wfloat-equal -Wformat-nonliteral -Wformat-security       \
	   -Wformat-signedness -Wformat=2 -Winline -Wlogical-op      \
	   -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual     \
	   -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls    \
		 -Wshadow -Wsign-conversion -Wsign-promo                   \
		 -Wstrict-null-sentinel -Wstrict-overflow=2                \
		 -Wsuggest-attribute=noreturn -Wsuggest-final-methods      \
		 -Wsuggest-final-types -Wsuggest-override -Wswitch-default \
		 -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code      \
		 -Wunused -Wuseless-cast -Wvariadic-macros                 \
		 -Wno-literal-suffix -Wno-missing-field-initializers       \
		 -Wno-narrowing -Wno-old-style-cast -Wno-varargs           \
		 -fcheck-new -fsized-deallocation -fstrict-overflow        \
		 -Wstack-usage=8192 -pie -fPIE -Werror=vla")
set(CMAKE_CXX_FLAGS_DEBUG
    "-O0 -ggdb                                                     \
     -Wstack-protector -fstack-protector                           \
		 -fno-omit-frame-pointer                                       \
     -fsanitize=address,alignment,bool,bounds,enum                 \
     -fsanitize=float-cast-overflow,float-divide-by-zero,          \
     -fsanitize=integer-divide-by-zero,leak,nonnull-attribute,     \
     -fsanitize=null,object-size,return,returns-nonnull-attribute, \
     -fsanitize=shift,signed-integer-overflow,undefined,           \
     -fsanitize=unreachable,vla-bound,vptr")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# Executable requirements
add_executable(${PROJECT_NAME})

target_sources(
  ${PROJECT_NAME}

  PRIVATE
    src/main.cpp src/render.cpp src/geometry.cpp
    src/glad.cpp
)

target_link_libraries(
  ${PROJECT_NAME}

  PRIVATE
    OpenGL::GL
    glm::glm
    glfw
)
