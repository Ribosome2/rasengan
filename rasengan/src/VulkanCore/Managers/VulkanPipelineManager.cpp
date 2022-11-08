#include "VulkanPipelineManager.h"
std::vector<VulkanPipeline*> VulkanPipelineManager::pipelines ;

void VulkanPipelineManager::RecreateAllPipelines() {
	for (auto pipeline : pipelines) {
		pipeline->RecreatePipeline();
	}
}
