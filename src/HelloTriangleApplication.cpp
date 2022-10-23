//
// Created by ianpo on 12/10/2022.
//

#include "HelloTriangleApplication.hpp"
#include <cstring>

#define LOG(s) std::cout << s << std::endl;
#define LOG_ERR(s) std::cerr << s << std::endl;

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
        if (enableValidationLayers && !checkValidationLayerSupport()){
            throw std::runtime_error("The validation layers were requested, but not available!");
        }

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


        if (enableValidationLayers){
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        } else {
            createInfo.enabledLayerCount = 0;
        }

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
    }

    void HelloTriangleApplication::checkRequiredVulkanExtensions() {
        // Getting all the available extensions
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());


        // Getting the required extensions
        uint32_t requiredExtensionsCount = 0;
        const char** requiredExtensions = glfwGetRequiredInstanceExtensions(&requiredExtensionsCount);

        // Checking that we have the required extensions
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

        LOG("===== Found all the required extensions =====");
    }

    bool HelloTriangleApplication::checkValidationLayerSupport(){
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : validationLayers) {
            bool layerFound = false;

            for (const auto& layerProperty : availableLayers) {
                if(strcmp(layerName, layerProperty.layerName) == 0){
                    // Too many logs, disable this one, but can be enabled if needed.
                    // LOG("The layer \"" << layerName << "\" was found.");
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound){
                LOG_ERR("The layer\"" << layerName << "\" wasn't found.");
                return false;
            }
        }
        return true;
    }

    void HelloTriangleApplication::mainLoop() {
        while (!glfwWindowShouldClose(window)){
            glfwPollEvents();
        }
    }

    void HelloTriangleApplication::cleanup() {
        //The VkInstance should be only destroyed right before the program exits. It
        //can be destroyed in cleanup with the vkDestroyInstance function
        vkDestroyInstance(instance, nullptr);

        glfwDestroyWindow(window);

        glfwTerminate();
    }
} // vkl