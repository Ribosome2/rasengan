#include "VulkanDevice.h"
#include <vector>
#include <stdexcept>
#include <iostream>
#include "VulkanContext.h"
#include "assert.h"
bool VulkanDevice::isDeviceSuitable(VkPhysicalDevice device) {
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    QueueFamilyIndices indices = findQueueFamilies(device);
    return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
           deviceFeatures.geometryShader && indices.isComplete();
}

void VulkanDevice::PickPhysicalDevice(VkInstance instance) {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    for (const auto &device: devices) {
        if (isDeviceSuitable(device)) {
            physicalDevice = device;
            break;
        }
    }

    if (physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("failed to find a suitable GPU!");
    }

    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
    std::cout << "---picked device: " << deviceProperties.deviceName << std::endl;
}

QueueFamilyIndices VulkanDevice::findQueueFamilies(VkPhysicalDevice device) {
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto &  queueFamily : queueFamilies)
    {
        if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphicsFamily = i;
            VkBool32 presentSupport = false;
            auto   vkContext = VulkanContext::Get();
            assert(vkContext->SwapChain->surface!=VK_NULL_HANDLE); //need to make sure the surface is created first
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, vkContext->SwapChain->surface, &presentSupport);
            if(presentSupport)
            {
                indices.presentFamily = i;
            }
        }
        i++;
    }
    return indices;
}

void VulkanDevice::CreateLogicDevice(VulkanValidation & vulkanValidation  ) {
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
    queueCreateInfo.queueCount = 1;

    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures{};
    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = 0;

    if(vulkanValidation.enableValidationLayers)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(vulkanValidation.validationLayers.size());
        createInfo.ppEnabledLayerNames = vulkanValidation.validationLayers.data();
    }else
    {
        createInfo.enabledLayerCount = 0;
    }

    if(vkCreateDevice(physicalDevice,&createInfo,nullptr,&device)!=VK_SUCCESS)
    {
        throw std::runtime_error("failed to create logical device!");
    }else{
        std::cout<<"logicalDevice created "<<std::endl;
    }

    vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
}

VulkanDevice::~VulkanDevice() {
    std::cout<<"Vulkan Device dispose "<<std::endl;
}
