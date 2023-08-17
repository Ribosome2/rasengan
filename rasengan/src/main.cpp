
// sys headers
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#define GLFW_INCLUDE_VULKAN
#define STB_IMAGE_IMPLEMENTATION
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
#include "Input/Input.h"
#include "DemoScene.h"
#include "EditorCore/HierarchyWindow.h"
#include "EditorCore/InspectorWindow.h"
#include "EngineCore/Logger.h"
#include "EngineCore/AssertUtil.h"

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
	const uint32_t WIDTH = 1280;
	const uint32_t HEIGHT = 720;
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
		VkSampleCountFlagBits MSAASamples = vkContext->VulkanDevice->msaaSamples;
		uint32_t imageCount = 2;
		VulkanImguiLayer imguiLayer{vkContext->window, vkContext->renderPass, imageCount, MSAASamples};

        DemoScene demoScene;
        std::vector<std::shared_ptr<GameObject>> & gameObjects =demoScene.gameObjects ;
		HierarchyWindow hierarchyWindow;
		InspectorWindow inspectorWindow;
        Input::Init(vkContext->window);
		while (!glfwWindowShouldClose(mVulkanWindows.window)) {
			glfwPollEvents();
            Input::Update();


			Time::Update();
			imguiLayer.NewFrame();

            Input::OnGUI();
			hierarchyWindow.OnGUI(&demoScene);
			InspectorWindow::OnInspectorGUI();
			vkContext->VulkanRenderer->Update();
			for (auto & go : gameObjects) {
				go->Update();
			}
            vkContext->VulkanRenderer->BeginFrame();
			{
                vkContext->VulkanRenderer->DrawFrame(gameObjects);
//                imguiLayer.OnGui();

				imguiLayer.Render(vkContext->CommandBuffer.GetCurCommandBuffer());
			}
            vkContext->VulkanRenderer->EndFrame();
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
    RFATAL("A test message: %f", 3.14f);
    RERROR("A test message: %f", 3.14f);
    RWARN("A test message: %f", 3.14f);
    RINFO("A test message: %f", 3.14f);
    RDEBUG("A test message: %f", 3.14f);
    RTRACE("A test message: %f", 3.14f);

//    RASSERT(1 == 0);
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