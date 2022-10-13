#pragma once


#include <stb_image.h>

class VulkanTexture {
public:
    VulkanTexture(std::string texturePath);

    virtual ~VulkanTexture();

    static VkImageView CreateImageView(VkImage & image, VkFormat format,VkImageAspectFlags aspectFlags);
    static void CreateImage(uint32_t width,
                            uint32_t height,
                            VkFormat format,
                            VkImageTiling tiling,
                            VkImageUsageFlags usage,
                            VkMemoryPropertyFlags properties,
                            VkImage &image,
                            VkDeviceMemory &imageMemory);
    static void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

public:
    VkImageView textureImageView;
    VkSampler textureSampler;
private:

    static bool hasStencilComponent(VkFormat format);

    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

    void createTextureSampler();

    VkImage textureImage;
    VkDeviceMemory textureImageMemory;


};
