#pragma once

#include "memory"
#include "VulkanSwapChain.h"
#include "vulkan/vulkan.h"
#include "VulkanDevice.h"
#include "VulkanCommandBuffer.h"
#include "EngineCore/Material.h"
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
	VkRenderPass renderPass = VK_NULL_HANDLE;
	VulkanDevice *VulkanDevice;
	std::shared_ptr<VulkanRenderer> VulkanRenderer;
	GLFWwindow *window;
private:
	// Vulkan instance
	inline static VkInstance s_VulkanInstance;

};