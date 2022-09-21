
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
#include "VulkanCore/VulkanVertex.h"
#include "VulkanCore/VulkanVertexBuffer.h"
#include "VulkanCore/VulkanIndexBuffer.h"

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
	void EditVertexData( std::vector<VulkanVertex> & vertices){
		for (int i = 0; i < vertices.size(); ++i) {
			auto & vertex  = vertices[i];
			ImGui::PushID(i);
			ImGui::SliderFloat2(("Position "),(float*)&vertex.pos,-1.5,2.5);
			ImGui::SliderFloat3("Color ",(float*)&vertex.color,-2,2);
			ImGui::PopID();
		}

	}

    void MainLoop() {
        auto vkContext = VulkanContext::Get();
        VkSampleCountFlagBits MSAASamples = VK_SAMPLE_COUNT_1_BIT;//todo: use value match our rendering
        uint32_t imageCount = 2;
        VulkanImguiLayer imguiLayer{vkContext->window, vkContext->renderPass, imageCount, MSAASamples};
        VulkanRenderer vulkanRenderer{};
        vulkanRenderer.Init();
        VulkanShader testShader("shaders/vertexBuffer.vert", "shaders/simpleColor.frag");
        VulkanPipeline pipeline(testShader);
        m_VulkanContext->graphicsPipeline = &pipeline.graphicsPipeline;
        m_VulkanContext->wireframePipeline = &pipeline.wireFramePipeline;
		 std::vector<VulkanVertex> vertices = {
				 {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
				 {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
				 {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
				 {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
		};
		 std::vector<uint16_t> indices = {
				0, 1, 2, 2, 3, 0
		};
        auto vertexBufferSize = sizeof(vertices[0])*vertices.size();
        VulkanVertexBuffer vertexBuffer(vertices.data(),static_cast<uint32_t>(vertexBufferSize));

		auto indexBufferSize = sizeof(indices[0])*indices.size();
		VulkanIndexBuffer indexBuffer(indices.data(),indices.size(),static_cast<uint32_t>(indexBufferSize));
		vulkanRenderer.RenderContext.vertexBuffer = &vertexBuffer;
		vulkanRenderer.RenderContext.indexBuffer = &indexBuffer;

        while (!glfwWindowShouldClose(mVulkanWindows.window)) {
            glfwPollEvents();
            imguiLayer.NewFrame();
            vulkanRenderer.UpdateUniformBuffer();
            vulkanRenderer.BeginFrame();
            {
                vulkanRenderer.DrawFrame();
//                imguiLayer.OnGui();
				EditVertexData(vertices);
				vertexBuffer.UpdateMemory();
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