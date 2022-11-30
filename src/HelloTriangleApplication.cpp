//
// Created by ianpo on 12/10/2022.
//

#include "HelloTriangleApplication.hpp"
#include "QueueFamilyIndices.hpp"
#include <cstring>

#define LOG(s) std::cout << s << std::endl;
#define LOG_WARN(s) std::cout << s << std::endl;
#define LOG_ERR(s) std::cerr << s << std::endl;

namespace vkl {
    /// Proxy function to automatically load a VkDebugUtilsMessengerCreateInfoEXT.
    /// \param instance
    /// \param pCreateInfo
    /// \param pAllocator
    /// \param pDebugMessenger
    /// \return
    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
                                          const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                          const VkAllocationCallbacks* pAllocator,
                                          VkDebugUtilsMessengerEXT* pDebugMessenger) {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance,"vkCreateDebugUtilsMessengerEXT");
        if(func != nullptr) {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        } else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    void DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                           VkDebugUtilsMessengerEXT debugMessenger,
                                           const VkAllocationCallbacks* pAllocator) {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance,"vkDestroyDebugUtilsMessengerEXT");
        if(func != nullptr) {
            func(instance, debugMessenger, pAllocator);
        }
    }

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
        setupDebugMessenger();
        pickPhysicalDevice();
        createLogicalDevice();
    }

    void HelloTriangleApplication::pickPhysicalDevice() {
        physicalDevice = VK_NULL_HANDLE;

        // Listing the available graphics cards
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
        if(deviceCount == 0) {
            throw std::runtime_error("Failed to find GPUs with Vulkan support!");
        }

        //Get all the devices possible
        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

        // Select the first suitable device
        // TODO: allow the change for a better one
        for (const auto& device : devices) {
            if(isDeviceSuitable(device)){
                physicalDevice = device;
                break;
            }
        }

        if(physicalDevice == VK_NULL_HANDLE) {
            throw std::runtime_error("Failed to find a suitable GPU.");
        }
    }

    void HelloTriangleApplication::createLogicalDevice() {
        // Find Queue with graphics capabilities:
        QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

        // We only created one queue family.
        VkDeviceQueueCreateInfo qUeueCreateInfo{};
        qUeueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        qUeueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
        qUeueCreateInfo.queueCount = 1;

        // /!\ Mandatorty /!\
        // Setting priority
        float queuePriority = 1.0f;
        qUeueCreateInfo.pQueuePriorities = &queuePriority;

        // Empty for now as we don't need anything special
        VkPhysicalDeviceFeatures deviceFeatures{};

        // Filling the main struct.
        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = 1;
        createInfo.pEnabledFeatures = &deviceFeatures;

        // Difference between instance & device specific layer is no longer a thing.
        // But still doing it for retro-compatibility
        createInfo.enabledExtensionCount = 0;
        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        } else {
            createInfo.enabledLayerCount = 0;
        }
        if(vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create logical device.");
        }
    }

    // TODO: Implement a system taking a configuration file to know which device are suitable and
    //  make an algorithm to filter the GPU and take the best. (and allow the user to change it for a)
    //  a specific one if he want to.
    bool HelloTriangleApplication::isDeviceSuitable(VkPhysicalDevice device) {
        QueueFamilyIndices indices = vkl::findQueueFamilies(device);

        return indices.isComplete();
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
            auto extensions = getRequiredExtensions();
            createInfo.enabledLayerCount = static_cast<uint32_t>(extensions.size());
            createInfo.ppEnabledLayerNames = extensions.data();
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

    std::vector<const char*> HelloTriangleApplication::getRequiredExtensions() {
        uint32_t glfwExtensionsCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionsCount);

        if (enableValidationLayers) {
            for (const char* validationLayer:validationLayers) {
                extensions.push_back(validationLayer);
            }
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }
        return extensions;
    }


    VkBool32 HelloTriangleApplication::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                     VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                     const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                                     void *pUserData) {

        if(messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
            // The message is important enough to be shown.
            LOG_ERR("validation layer: " << pCallbackData->pMessage);
        }

        return VK_FALSE;
    }

    void HelloTriangleApplication::setupDebugMessenger() {
        if(!enableValidationLayers) return;

        VkDebugUtilsMessengerCreateInfoEXT createInfo {};
        // Just the type of structure this is.
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

        // The type of severity I want to call with this messenger.
        createInfo.messageSeverity =
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

        // The type of message I want to be notified about.
        createInfo.messageType =
                VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

        // The function it should call to pass all that.
        createInfo.pfnUserCallback = debugCallback;

        // Optional: A pointer that will be passed to the callback when called.
        createInfo.pUserData = nullptr;

        // This is an extension function, so we have to search for it first
        if(CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
            throw std::runtime_error("failed to set up debug messenger!");
        }
    }

    void HelloTriangleApplication::mainLoop() {
        while (!glfwWindowShouldClose(window)){
            glfwPollEvents();
        }
    }

    void HelloTriangleApplication::cleanup() {
        if(enableValidationLayers) {
            DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
        }

        vkDestroyDevice(device, nullptr);

        //The VkInstance should be only destroyed right before the program exits. It
        //can be destroyed in cleanup with the vkDestroyInstance function
        vkDestroyInstance(instance, nullptr);

        glfwDestroyWindow(window);

        glfwTerminate();
    }
} // vkl