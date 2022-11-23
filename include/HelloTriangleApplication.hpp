//
// Created by ianpo on 12/10/2022.
//

#ifndef VULKANLEARNING_HELLOTRIANGLEAPPLICATION_HPP
#define VULKANLEARNING_HELLOTRIANGLEAPPLICATION_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "iostream"
#include <stdexcept>
#include <vector>
#include <cstdlib>



namespace vkl {

    class HelloTriangleApplication {
    public:
        void run();
        const uint32_t VKL_WINDOW_WIDTH = 800;
        const uint32_t VKL_WINDOW_HEIGHT = 600;
        const char* VKL_WINDOW_NAME = "Vulkan Window";

        const std::vector<const char*> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };
#ifdef NDEBUG
        const bool enableValidationLayers = false
#else
        const bool enableValidationLayers = true;
#endif
    private:
        void initWindow();
        void initVulkan();
        void mainLoop();
        void cleanup();

        void createInstance();

        // Function that I create as part of the tutorial
        void checkRequiredVulkanExtensions();

        bool checkValidationLayerSupport();
        std::vector<const char*> getRequiredExtensions();

        // The vulkan instance
        VkInstance instance;
        // The selected graphics card
        VkPhysicalDevice physicalDevice;
        GLFWwindow* window;
        // Debug variable (?)
        VkDebugUtilsMessengerEXT debugMessenger;

        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
                VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                VkDebugUtilsMessageTypeFlagsEXT messageType,
                const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                void* pUserData);

        void setupDebugMessenger();

        void pickPhysicalDevice();
        bool isDeviceSuitable(VkPhysicalDevice device);
    };

} // vkl

#endif //VULKANLEARNING_HELLOTRIANGLEAPPLICATION_HPP
