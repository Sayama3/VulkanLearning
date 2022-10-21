//
// Created by ianpo on 12/10/2022.
//

#include <vector>
#include <cstring>
#include "HelloTriangleApplication.hpp"

#define LOG(s) std::cout << s << '\n'

namespace vkl {
    void HelloTriangleApplication::run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

    void HelloTriangleApplication::initWindow() {
        glfwInit();

        // We don't want the OpenGL API
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        // We will later on handle the resizing of the window
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(VKL_WINDOW_WIDTH, VKL_WINDOW_HEIGHT, VKL_WINDOW_NAME, nullptr, nullptr);
    }

    void HelloTriangleApplication::initVulkan() {
        createInstance();
    }

    void HelloTriangleApplication::createInstance() {
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1,0,0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1,0,0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        // Check if we have all the extensions we need.
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        // Set all the extensions we need.
        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;

        createInfo.enabledLayerCount = 0;

        //Creating the instance of Vulkan
        if(vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS){
            throw std::runtime_error("failed to create vulkan instance.");
        }

        // Getting all the available extensions
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

        // Getting the required extensions
        uint32_t requiredExtensionsCount = 0;
        const char** requiredExtensions = glfwGetRequiredInstanceExtensions(&requiredExtensionsCount);

        // CHecking that we have the required extensions
        for (int i = 0; i < requiredExtensionsCount; ++i) {
            bool found = false;
            const char* requiredExtension = requiredExtensions[i];
            for (const auto& extension: extensions) {
                if (strcmp(extension.extensionName, requiredExtension) == 0){
                    LOG("Found " << requiredExtension << '.');
                    found = true;
                    break;
                }
            }

            // If we miss one, throwing error.
            if (!found){
                throw std::runtime_error(std::string(requiredExtension).append(" not found."));
            }
        }

    }

    void HelloTriangleApplication::mainLoop() {
        while (!glfwWindowShouldClose(window)){
            glfwPollEvents();
        }
    }

    void HelloTriangleApplication::cleanup() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
} // vkl