#pragma once


#include <stb_image.h>

class VulkanTexture {
public:
    VulkanTexture();

    virtual ~VulkanTexture();

    static VkImageView CreateImageView(VkImage image, VkFormat format);

private:
    void createImage(uint32_t width,
                     uint32_t height,
                     VkFormat format,
                     VkImageTiling tiling,
                     VkImageUsageFlags usage,
                     VkMemoryPropertyFlags properties,
                     VkImage &image,
                     VkDeviceMemory &imageMemory);

    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
    void createTextureSampler();

    VkImage textureImage;
    VkDeviceMemory textureImageMemory;

    VkImageView textureImageView;

    VkSampler textureSampler;
};
