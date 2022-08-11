
// sys headers
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "VulkanCore/VulkanApp.h"
#include "VulkanCore/VulkanContext.h"
#include <cassert>
// Use (void) to silence unused warnings.
#define assertm(exp, msg) assert(((void)msg, exp))

VulkanContext* VulkanContext::mContextInstance;

class RasenganApp
{
public:
	void Run()
	{
        std::cout << "Rasengan start " << std::endl;
        Init();
        MainLoop();
        Cleanup();
	}

private:
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;
    VulkanApp mVulkanApp;
    VulkanWindow  mVulkanWindows;
    VulkanContext m_VulkanContext;
	void Init()
	{
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        mVulkanWindows.window = glfwCreateWindow(WIDTH, HEIGHT, "VulkanRasengan", nullptr, nullptr);

        if (!mVulkanWindows.window)
        {
            glfwTerminate();
            throw std::runtime_error("failed to create vkInstance");
        }
        VulkanContext::SetInstance(m_VulkanContext);
        VulkanContext::GetInstance().Init();
        mVulkanApp.CreateInstance();
	}
	void MainLoop()
	{
        while (!glfwWindowShouldClose(mVulkanWindows.window)) {
            glfwSwapBuffers(mVulkanWindows.window);
            glfwPollEvents();
        }
	}
	void Cleanup()
	{
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
    catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}