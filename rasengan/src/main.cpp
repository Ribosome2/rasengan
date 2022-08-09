
// sys headers
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "VulkanCore/VulkanApp.h"
#include <cassert>
// Use (void) to silence unused warnings.
#define assertm(exp, msg) assert(((void)msg, exp))
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
    GLFWwindow* window;
    VulkanApp mVulkanApp;
	void Init()
	{
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        window = glfwCreateWindow(WIDTH, HEIGHT, "VulkanRasengan", nullptr, nullptr);

        if (!window)
        {
            glfwTerminate();
            throw std::runtime_error("failed to create vkInstance");
        }

        mVulkanApp.CreateInstance();
	}
	void MainLoop()
	{
        while (!glfwWindowShouldClose(window)) {
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
	}
	void Cleanup()
	{
        mVulkanApp.Cleanup();
        glfwDestroyWindow(window);

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