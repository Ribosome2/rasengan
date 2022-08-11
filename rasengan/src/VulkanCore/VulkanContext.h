#pragma once
#include "memory"
#include "VulkanSwapChain.h"
#include "vulkan/vulkan.h"
class VulkanContext{
private:
    static VulkanContext* mContextInstance;
public:
    static VulkanContext &  GetInstance(){
        return *mContextInstance;
    }

    static void  SetInstance(VulkanContext & context){
        mContextInstance=&context;
    }

    void Init(){
        SwapChain = std::make_shared<VulkanSwapChain>();
    }
    std::shared_ptr<VulkanSwapChain> SwapChain;

};