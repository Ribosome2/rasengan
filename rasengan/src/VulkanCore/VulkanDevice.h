#pragma once

#include <vulkan/vulkan.h>
#include <optional>
#include "VulkanValidation.h"

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;

    bool isComplete() {
        return graphicsFamily.has_value();
    }
};

class VulkanDevice {
public:
    void PickPhysicalDevice(VkInstance instance);

    void CreateLogicDevice(VulkanValidation &vulkanValidation);
public:
    VkDevice device = VK_NULL_HANDLE;

private:
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    bool isDeviceSuitable(VkPhysicalDevice device);

private:
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

    VkQueue graphicsQueue = VK_NULL_HANDLE;
};
