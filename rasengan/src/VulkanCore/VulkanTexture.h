#pragma once


#include <stb_image.h>

class VulkanTexture {
public:
    VulkanTexture();

private:
    void createImage(uint32_t width,
                     uint32_t height,
                     VkFormat format,
                     VkImageTiling tiling,
                     VkImageUsageFlags usage,
                     VkMemoryPropertyFlags properties,
                     VkImage &image,
                     VkDeviceMemory &imageMemory);

    VkImage textureImage;
    VkDeviceMemory textureImageMemory;
};
