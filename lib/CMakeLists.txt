cmake_minimum_required(VERSION 3.23)

add_subdirectory(glfw)
add_subdirectory(glm)
add_subdirectory(vulkan)

find_package(Vulkan REQUIRED FATAL_ERROR)
target_include_directories(VulkanLearning PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(VulkanLearning PUBLIC glfw glm ${Vulkan_LIBRARIES})