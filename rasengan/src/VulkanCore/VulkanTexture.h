#pragma once


#include <stb_image.h>

class VulkanTexture {
public:
    VulkanTexture(std::string texturePath);

    virtual ~VulkanTexture();

    static VkImageView CreateImageView(VkImage image, VkFormat format);

public:
    VkImageView textureImageView;
    VkSampler textureSampler;
private:
    static void createImage(uint32_t width,
                     uint32_t height,
                     VkFormat format,
                     VkImageTiling tiling,
                     VkImageUsageFlags usage,
                     VkMemoryPropertyFlags properties,
                     VkImage &image,
                     VkDeviceMemory &imageMemory);

    static void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

    void createTextureSampler();

    VkImage textureImage;
    VkDeviceMemory textureImageMemory;


};
