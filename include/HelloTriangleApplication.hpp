//
// Created by ianpo on 12/10/2022.
//

#ifndef VULKANLEARNING_HELLOTRIANGLEAPPLICATION_HPP
#define VULKANLEARNING_HELLOTRIANGLEAPPLICATION_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "iostream"
#include <stdexcept>
#include <cstdlib>



namespace vkl {

    class HelloTriangleApplication {
    public:
        void run();
        const uint32_t VKL_WINDOW_WIDTH = 800;
        const uint32_t VKL_WINDOW_HEIGHT = 600;
        const char* VKL_WINDOW_NAME = "Vulkan Window";
    private:
        void initWindow();
        void initVulkan();
        void mainLoop();
        void cleanup();

        void createInstance();

        VkInstance instance;
        GLFWwindow* window;
    };

} // vkl

#endif //VULKANLEARNING_HELLOTRIANGLEAPPLICATION_HPP
