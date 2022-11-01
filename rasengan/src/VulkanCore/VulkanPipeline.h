#pragma once

#include <vulkan/vulkan.h>
#include "VulkanShader.h"

class VulkanPipeline {
public:
	VulkanPipeline(VulkanShader &shader);

	~VulkanPipeline();

private:
public:
	VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
	VkPipeline graphicsPipeline = VK_NULL_HANDLE;
	VkPipeline wireFramePipeline = VK_NULL_HANDLE;
private:
};
