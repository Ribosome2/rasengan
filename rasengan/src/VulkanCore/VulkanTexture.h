#pragma once


#include <stb_image.h>

class VulkanTexture {
public:
	VulkanTexture(std::string texturePath);

	virtual ~VulkanTexture();

	int GetWidth() { return m_width; };

	int GetHeight() { return m_height; };

	static VkImageView
	CreateImageView(VkImage &image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
    void UpdateImageViewAndSampler();
	static void CreateImage(uint32_t width,
							uint32_t height,
							uint32_t mipLevels,
							VkSampleCountFlagBits numSamples,
							VkFormat format,
							VkImageTiling tiling,
							VkImageUsageFlags usage,
							VkMemoryPropertyFlags properties,
							VkImage &image,
							VkDeviceMemory &imageMemory);

	static void ReloadTexture(std::string texturePath,VulkanTexture * t);

	static void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout,
									  uint32_t mipLevels = 1);

	static void
	GenerateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);

public:
	VkImageView textureImageView = VK_NULL_HANDLE;
	VkSampler textureSampler = VK_NULL_HANDLE;
	VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
    int Generation=0;
private:

	static bool hasStencilComponent(VkFormat format);

	static void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

	void createTextureSampler();

	int m_width = 0;
	int m_height = 0;
	uint32_t m_mipLevels = 0;
	VkImage textureImage = VK_NULL_HANDLE;
	VkDeviceMemory textureImageMemory = VK_NULL_HANDLE;


};
