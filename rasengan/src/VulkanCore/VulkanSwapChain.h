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
    uint32_t GetCurrentBufferIndex(){ return currentBufferIndex;};
    VkFramebuffer &  GetCurrentFrameBuffer(){ return swapChainFramebuffers[currentBufferIndex];};
private:
    void createFramebuffers();
    void createRenderPass();
    void createSyncObjects();
public:

    VkSurfaceKHR surface;
    VkSwapchainKHR swapChain;
    VkExtent2D swapChainExtent;
    VkFormat swapChainImageFormat;
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkImage> swapChainImages;
    std::vector<VkFramebuffer> swapChainFramebuffers;
    uint32_t currentBufferIndex=0;
    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;
    VkFence inFlightFence;
private:
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

};
