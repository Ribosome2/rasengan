#pragma once


#include <stb_image.h>

class VulkanTexture {
public:
    VulkanTexture(std::string texturePath);

    virtual ~VulkanTexture();

    static VkImageView CreateImageView(VkImage & image, VkFormat format,VkImageAspectFlags aspectFlags,uint32_t mipLevels);
    static void CreateImage(uint32_t width,
                            uint32_t height,
                            uint32_t mipLevels,
                            VkFormat format,
                            VkImageTiling tiling,
                            VkImageUsageFlags usage,
                            VkMemoryPropertyFlags properties,
                            VkImage &image,
                            VkDeviceMemory &imageMemory);
    static void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout,uint32_t mipLevels=1);

public:
    VkImageView textureImageView= nullptr;
    VkSampler textureSampler= nullptr;
private:

    static bool hasStencilComponent(VkFormat format);

    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

    void createTextureSampler();

    uint32_t m_mipLevels;
    VkImage textureImage= VK_NULL_HANDLE;
    VkDeviceMemory textureImageMemory= VK_NULL_HANDLE;


};
