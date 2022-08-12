#pragma once
#include "memory"
#include "VulkanSwapChain.h"
#include "vulkan/vulkan.h"
#include "VulkanDevice.h"
class VulkanContext{
private:
    static VulkanContext* mContextInstance;
public:
    //todo : switch to shared_ptr?
    // "auto vkContext =VulkanContext::GetContext();" may cause problem
    //  but  "auto & vkContext =VulkanContext::GetContext();" is ok
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
    VulkanDevice* VulkanDevice;
    GLFWwindow* window;
private:
    // Vulkan instance
    inline static VkInstance s_VulkanInstance;

};