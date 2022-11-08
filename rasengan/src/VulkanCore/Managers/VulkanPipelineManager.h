#pragma once
#include "VulkanCore/VulkanPipeline.h"

class VulkanPipelineManager {
public:
	static std::vector<VulkanPipeline*> pipelines;
	static void RecreateAllPipelines();
};
