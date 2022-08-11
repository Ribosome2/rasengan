#include <stdexcept>
#include "VulkanSwapChain.h"
#include "VulkanContext.h"
#include "iostream"
void VulkanSwapChain::InitSurface(GLFWwindow *window) {
    if (glfwCreateWindowSurface(VulkanContext::GetVulkanInstance(), window, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }else{
        std::cout<<"glfwCreateWindowSurface success. " <<std::endl;
    }
}
