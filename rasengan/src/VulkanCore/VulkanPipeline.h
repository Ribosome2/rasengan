#pragma once
#include <vulkan/vulkan.h>
#include "VulkanContext.h"
#include "VulkanShader.h"
class VulkanPipeline {
public:
    VulkanPipeline(VulkanShader & shader);
private:
public:
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;
    VkRenderPass renderPass;
private:
    void createRenderPass();
};
