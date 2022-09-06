#pragma once

#include <vulkan/vulkan.h>
#include <optional>
#include "VulkanValidation.h"
#include "VulkanRenderer.h"

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;
    bool isComplete() const {
        //todo: check presentSupport
        return graphicsFamily.has_value() ;
    }
};

class VulkanDevice {
public:
    virtual ~VulkanDevice();
    void Dispose();
    void PickPhysicalDevice(VkInstance instance);

    void CreateLogicDevice(VulkanValidation &vulkanValidation);
    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

public:
    VkDevice device = VK_NULL_HANDLE;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;


private:

    bool isDeviceSuitable(VkPhysicalDevice device);

    VkQueue graphicsQueue = VK_NULL_HANDLE;
    VkQueue presentQueue = VK_NULL_HANDLE;
    const std::vector<const char*> deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
    friend VulkanRenderer;
};
