#pragma once
#include "vulkan/vulkan.h"
#include "GLFW/glfw3.h"
#include <vector>

struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class VulkanSwapChain {
public:
	virtual ~VulkanSwapChain();

	void InitSurface(GLFWwindow* windowHandle);
    void CreateSwapchain();
    void CreateImageViews();
    VkFramebuffer &  GetCurrentFrameBuffer(uint32_t imageIndex ){ return swapChainFramebuffers[imageIndex];};
private:
    void createFramebuffers();
    void createRenderPass();
    void createSyncObjects();
	void createDepthResources();
	VkFormat findDepthFormat();
	bool hasStencilComponent(VkFormat format);

public:

    VkSurfaceKHR surface= nullptr;
    VkSwapchainKHR swapChain= nullptr;
    VkExtent2D swapChainExtent;
    VkFormat swapChainImageFormat;
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkImage> swapChainImages;

	//for depth buffer
	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;

    std::vector<VkFramebuffer> swapChainFramebuffers;
    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;
    VkFence inFlightFence;
private:
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

};
