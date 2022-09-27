
// sys headers
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include "VulkanCore/VulkanApp.h"
#include "VulkanCore/VulkanContext.h"
#include <cassert>
#include "VulkanCore/VulkanShader.h"
#include "VulkanCore/VulkanPipeline.h"
#include "VulkanImguiLayer.hpp"
#include "VulkanCore/VulkanIndexBuffer.h"
#include "EngineCore/Time.h"
#include "EngineCore/GameObject.h"
#include "EngineCore/MeshRenderer.h"
#include "EngineCore/Material.h"

std::shared_ptr<VulkanContext> VulkanContext::mContextInstance;

class RasenganApp {
public:
	void Run() {
		std::cout << "Rasengan start " << std::endl;
		Init();
		MainLoop();
		Cleanup();
	}

private:
	const uint32_t WIDTH = 800;
	const uint32_t HEIGHT = 600;
	VulkanApp mVulkanApp;
	VulkanWindow mVulkanWindows;
	std::shared_ptr<VulkanContext> m_VulkanContext;

	void Init() {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		mVulkanWindows.window = glfwCreateWindow(WIDTH, HEIGHT, "VulkanRasengan", nullptr, nullptr);

		if (!mVulkanWindows.window) {
			glfwTerminate();
			throw std::runtime_error("failed to create vkInstance");
		}
		m_VulkanContext = std::make_shared<VulkanContext>();
		VulkanContext::SetInstance(m_VulkanContext);
		m_VulkanContext->Init();
		m_VulkanContext->window = mVulkanWindows.window;
		mVulkanApp.CreateInstance();


	}


	void MainLoop() {
		auto vkContext = VulkanContext::Get();
		VkSampleCountFlagBits MSAASamples = VK_SAMPLE_COUNT_1_BIT;//todo: use value match our rendering
		uint32_t imageCount = 2;
		VulkanImguiLayer imguiLayer{vkContext->window, vkContext->renderPass, imageCount, MSAASamples};
		VulkanRenderer vulkanRenderer{};
        vkContext->VulkanRenderer= &vulkanRenderer;
		vulkanRenderer.Init();
		VulkanShader testShader("shaders/vertexWithUniformBuffer.vert", "shaders/simpleColor.frag");
		VulkanPipeline pipeline(testShader);
        MeshRenderer meshRenderer;
        Material testMaterial;
        testMaterial.shader = &testShader;
        testMaterial.pipeline = &pipeline;
        testMaterial.CreateDescriptorSets(testShader.descriptorSetLayout);
        meshRenderer.material =testMaterial;

        vulkanRenderer.RenderContext.meshRenderer = &meshRenderer;
		vulkanRenderer.RenderContext.material = &testMaterial;
		GameObject quadGo;
        meshRenderer.transform = &quadGo.transform;
		while (!glfwWindowShouldClose(mVulkanWindows.window)) {
			glfwPollEvents();
			Time::Update();
			imguiLayer.NewFrame();
			vulkanRenderer.BeginFrame();
			{
                meshRenderer.Update();
				vulkanRenderer.DrawFrame();
//                imguiLayer.OnGui();

				imguiLayer.Render(vkContext->CommandBuffer.GetCurCommandBuffer());
			}
			vulkanRenderer.EndFrame();
			glfwSwapBuffers(mVulkanWindows.window);
		}
		vkDeviceWaitIdle(m_VulkanContext->VulkanDevice->device);
	}

	void Cleanup() {
		mVulkanApp.Cleanup();
		glfwDestroyWindow(mVulkanWindows.window);

		glfwTerminate();
	}
};

int main() {

	RasenganApp app;
	try {
		app.Run();
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}