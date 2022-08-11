#include <stdexcept>
#include "VulkanWindow.h"

void VulkanWindow::CreateSurface(VkInstance instance) {
    if(glfwCreateWindowSurface(instance,window,nullptr,&surface)!=VK_SUCCESS)
    {
        throw std::runtime_error("failed to create window surface!");
    }
}


