#pragma once

#include "VulkanApp.h"
#include "vector"
#include "VulkanValidation.h"
#include "VulkanContext.h"

void VulkanApp::CreateInstance() {
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Rasengan Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    createInfo.enabledLayerCount = 0;

    auto extensions = getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    createInfo.enabledLayerCount = 0;
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (mValidation.enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(mValidation.validationLayers.size());
        createInfo.ppEnabledLayerNames = mValidation.validationLayers.data();

        mValidation.PopulateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *) &debugCreateInfo;
    }


    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create vkInstance");
    }

    auto  vkContext = VulkanContext::Get();
    VulkanContext::SetVulkanInstance(instance);
    vkContext->SwapChain->InitSurface(vkContext->window);


    mValidation.setupDebugMessenger(instance);

    vkContext->VulkanDevice = &mVulkanDevice;
    mVulkanDevice.PickPhysicalDevice(instance);
    mVulkanDevice.CreateLogicDevice(mValidation);
    vkContext->SwapChain->CreateSwapchain();
    vkContext->CommandBuffer.Init();

}

void VulkanApp::Cleanup() {
    mValidation.Cleanup(instance);
	VulkanContext::Get()->SwapChain.reset() ;
	VulkanContext::Get()->CommandBuffer.Dispose() ;
    mVulkanDevice.Dispose();
    vkDestroyDevice(mVulkanDevice.device, nullptr);
    vkDestroyInstance(instance, nullptr);
}

std::vector<const char *> VulkanApp::getRequiredExtensions() {
    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
    mValidation.CheckAddValidationLayer(extensions);

    return extensions;
}

void VulkanApp::DrawFrame() {

}


