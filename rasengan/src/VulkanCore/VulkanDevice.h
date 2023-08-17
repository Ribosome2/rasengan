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
    uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	VkSampleCountFlagBits GetMaxUsableSampleCount();
public:
    VkDevice device = VK_NULL_HANDLE;
    VkQueue graphicsQueue = VK_NULL_HANDLE;
    VkQueue presentQueue = VK_NULL_HANDLE;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
private:

    bool isDeviceSuitable(VkPhysicalDevice device);


    const std::vector<const char*> deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
    friend VulkanRenderer;


};
