//
// Created by ianpo on 23/11/2022.
//

#ifndef VULKANLEARNING_QUEUEFAMILYINDICES_HPP
#define VULKANLEARNING_QUEUEFAMILYINDICES_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <optional>

namespace vkl {

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;

        bool isComplete();
    };

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

} // vkl

#endif //VULKANLEARNING_QUEUEFAMILYINDICES_HPP
