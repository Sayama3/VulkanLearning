# mark which minimum version of cmake is required
cmake_minimum_required(VERSION 3.23)

# set enable the debugger (when the debug mode is enable?)
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-g")

# set the name of the project
project(VulkanLearning)

# for visual studio : enable the math define
add_definitions(-D_USE_MATH_DEFINES)

# we use the c++ 20
set(CMAKE_CXX_STANDARD 20)

# set the name of the executable
add_executable(VulkanLearning)

# remark the c++ version to the 20 one
target_compile_features(VulkanLearning PUBLIC cxx_std_20)

# add the .hpp to the projects
include_directories(include)

# adding the subdirectories (all the subdirectories are self handles)
# which make adding external libraries using CMake extremely easy !
add_subdirectory(src)
add_subdirectory(lib)
