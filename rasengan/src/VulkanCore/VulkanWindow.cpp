#include <stdexcept>
#include "VulkanWindow.h"
#include "VulkanDebugUtil.h"

void VulkanWindow::CreateSurface(VkInstance instance) {
   VK_CHECK_RESULT(glfwCreateWindowSurface(instance,window,nullptr,&surface))
}


