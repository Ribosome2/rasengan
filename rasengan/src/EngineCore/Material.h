#pragma once

#include "VulkanCore/VulkanPipeline.h"
#include "VulkanCore/VulkanShader.h"

class Material {
public:
    Material();

    VulkanShader*  shader;
    VulkanPipeline *  graphicsPipeline;
};
