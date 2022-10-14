#include "VulkanPipeline.h"
#include "VulkanInitializer.h"
#include "VulkanVertex.h"
#include "VulkanContext.h"

VulkanPipeline::VulkanPipeline(VulkanShader &shader) {
	auto vkContext = VulkanContext::Get();
	VkPipelineVertexInputStateCreateInfo vertexInputInfo = VulkanInitializer::GetVertexInputStateCreateInfo();
    auto bindingDescription = VulkanVertex::GetBindingDescription();
    auto attributeDescriptions = VulkanVertex::GetAttributeDescriptions();

    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	VkPipelineInputAssemblyStateCreateInfo inputAssembly = VulkanInitializer::GetInputAssembleStateCreateInfo();

	//viewports and scissors
	auto &swapChainExtent = vkContext->SwapChain->swapChainExtent;
	VkViewport viewport = VulkanInitializer::GetViewPort((float) swapChainExtent.width,(float )swapChainExtent.height);

	VkRect2D scissor{};
	scissor.offset = {0, 0};
	scissor.extent = swapChainExtent;

	VkPipelineViewportStateCreateInfo viewportState = VulkanInitializer::ViewportStateCreateInfo(viewport, scissor);
	VkPipelineRasterizationStateCreateInfo rasterizer =VulkanInitializer::GetRasterizerCreateInfo();
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;

	VkPipelineMultisampleStateCreateInfo multisampling= VulkanInitializer::GetMultisampleStateCreateInfo();
	VkPipelineColorBlendAttachmentState colorBlendAttachment = VulkanInitializer::GetColorBlendAttachmentState();
	VkPipelineColorBlendStateCreateInfo colorBlending = VulkanInitializer::GetColorBlendStateCreateInfo(colorBlendAttachment);
	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = VulkanInitializer::GetPipelineLayoutCreateInfo();
    pipelineLayoutCreateInfo.setLayoutCount = 1;
    pipelineLayoutCreateInfo.pSetLayouts = &shader.descriptorSetLayout;

	if (vkCreatePipelineLayout(vkContext->VulkanDevice->device, &pipelineLayoutCreateInfo, nullptr, &pipelineLayout) !=
		VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout!");
	} else {
		std::cout << "created vulkan pipeline layout " << std::endl;
	}
    //Vulkan need to know what with change dynamically when creating  pipeline, or it won't respond to runtime change
    // for example ,if we don't tell vulkan VK_DYNAMIC_STATE_VIEWPORT ,vkCmdSetViewport won't have any effect on this pipeline
    std::vector<VkDynamicState> dynamicStateEnables = {
            VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR,
    };
    VkPipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo = VulkanInitializer::GetDynamicStateCreateInfo(dynamicStateEnables);

    VkPipelineDepthStencilStateCreateInfo depthStencil{};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.minDepthBounds = 0.0f; // Optional
    depthStencil.maxDepthBounds = 1.0f; // Optional
    depthStencil.stencilTestEnable = VK_FALSE;
    depthStencil.front = {}; // Optional
    depthStencil.back = {}; // Optional



	VkPipelineShaderStageCreateInfo shaderStages[] = {shader.vertShaderStageInfo, shader.fragShaderStageInfo};
	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = &depthStencil;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = &pipelineDynamicStateCreateInfo;
	pipelineInfo.layout = pipelineLayout;
	pipelineInfo.renderPass = vkContext->renderPass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
	pipelineInfo.basePipelineIndex = -1;//optional

	if (vkCreateGraphicsPipelines(vkContext->VulkanDevice->device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr,
								  &graphicsPipeline) != VK_SUCCESS) {
		throw std::runtime_error("failed to create graphics pipeline!");
	} else {
		std::cout << "created vkPipeline" << std::endl;
	}
	rasterizer.polygonMode = VK_POLYGON_MODE_LINE;

	if (vkCreateGraphicsPipelines(vkContext->VulkanDevice->device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr,
								  &wireFramePipeline) != VK_SUCCESS) {
		throw std::runtime_error("failed to create graphics pipeline!");
	} else {
		std::cout << "created vkPipeline" << std::endl;
	}


}

VulkanPipeline::~VulkanPipeline() {
	auto &device = VulkanContext::Get()->VulkanDevice->device;
	vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
	vkDestroyPipeline(device, graphicsPipeline, nullptr);
	vkDestroyPipeline(device, wireFramePipeline, nullptr);

}
