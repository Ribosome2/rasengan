
// sys headers
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class RasenganApp
{
public:
	void Run()
	{
        std::cout << "Rasengan start " << std::endl;
        MainLoop();
        Cleanup();
	}

private:
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;
    GLFWwindow* window;
	void Init()
	{
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        window = glfwCreateWindow(WIDTH, HEIGHT, "VulkanRasengan", nullptr, nullptr);

        if (!window)
        {
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
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