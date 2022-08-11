#pragma once
#include "memory"
#include "VulkanSwapChain.h"
#include "vulkan/vulkan.h"
class VulkanContext{
private:
    static VulkanContext* mContextInstance;
public:
    static VulkanContext &  GetContext(){
        return *mContextInstance;
    }

    static void  SetInstance(VulkanContext & context){
        mContextInstance=&context;
    }

    void Init(){
        SwapChain = std::make_shared<VulkanSwapChain>();
    }

    static VkInstance &  GetVulkanInstance(){
        return  s_VulkanInstance;
    }

    static void   SetVulkanInstance(VkInstance & instance ){
         s_VulkanInstance=instance;
    }

    std::shared_ptr<VulkanSwapChain> SwapChain;
    GLFWwindow* window;
private:
    // Vulkan instance
    inline static VkInstance s_VulkanInstance;

};