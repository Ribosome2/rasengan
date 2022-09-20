#pragma once
#include <vulkan/vulkan.h>
#include "VulkanContext.h"
#include "VulkanShader.h"
class VulkanPipeline {
public:
    VulkanPipeline(VulkanShader & shader);

	~VulkanPipeline();

private:
public:
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;
    VkPipeline wireFramePipeline;
private:
};
