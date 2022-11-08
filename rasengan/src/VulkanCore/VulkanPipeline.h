#pragma once

#include <vulkan/vulkan.h>
#include "VulkanShader.h"

class VulkanPipeline {
public:
	VulkanPipeline(VulkanShader &shader);

	~VulkanPipeline();
	void RecreatePipeline();

private:
	VulkanShader* m_Shader;
public:
	VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
	VkPipeline graphicsPipeline = VK_NULL_HANDLE;
	VkPipeline wireFramePipeline = VK_NULL_HANDLE;
private:
};
