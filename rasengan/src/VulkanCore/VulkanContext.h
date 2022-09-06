#pragma once

#include "memory"
#include "VulkanSwapChain.h"
#include "vulkan/vulkan.h"
#include "VulkanDevice.h"
#include "VulkanCommandBuffer.h"
//#include "VulkanPipeline.h"

class VulkanContext {
private:
    static std::shared_ptr<VulkanContext> mContextInstance;
public:
    static std::shared_ptr<VulkanContext> Get() {
        return mContextInstance;
    }

    static void SetInstance(std::shared_ptr<VulkanContext> context) {
        mContextInstance = context;
    }

    void Init() {
        SwapChain = std::make_shared<VulkanSwapChain>();
    }

    static VkInstance &GetVulkanInstance() {
        return s_VulkanInstance;
    }

    static void SetVulkanInstance(VkInstance &instance) {
        s_VulkanInstance = instance;
    }

    std::shared_ptr<VulkanSwapChain> SwapChain;
    VulkanCommandBuffer CommandBuffer;
    VkRenderPass renderPass;
    VulkanDevice *VulkanDevice;
    GLFWwindow *window;
    VkPipeline *graphicsPipeline;
private:
    // Vulkan instance
    inline static VkInstance s_VulkanInstance;

};