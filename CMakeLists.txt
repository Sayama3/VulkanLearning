cmake_minimum_required(VERSION 3.23)

set(CMAKE_CXX_FLAGS_DEBUG_INIT "-g")

project(VulkanLearning)

set(CMAKE_CXX_STANDARD 17)

add_executable(VulkanLearning)

target_compile_features(VulkanLearning PUBLIC cxx_std_20)

include_directories(include)

add_subdirectory(src)

# add_subdirectory(lib)