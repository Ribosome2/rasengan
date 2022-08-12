#include "VulkanShader.h"
#include "ShaderUtil.h"

VulkanShader::VulkanShader(std::string vertexPath, std::string fragPath) {
    auto vertShaderCode = ShaderUtil::ReadFile(vertexPath);
    auto fragShaderCode = ShaderUtil::ReadFile(fragPath);

    auto vkContext =VulkanContext::GetContext();
    vertShaderModule = ShaderUtil::CreateShaderModule(vkContext.VulkanDevice->device, vertShaderCode);
    fragShaderModule = ShaderUtil::CreateShaderModule(vkContext.VulkanDevice->device, fragShaderCode);

    vertShaderStageInfo = ShaderUtil::GetShaderStageCreateInfo(vertShaderModule, VK_SHADER_STAGE_VERTEX_BIT);
    fragShaderStageInfo = ShaderUtil::GetShaderStageCreateInfo(fragShaderModule, VK_SHADER_STAGE_FRAGMENT_BIT);
}
