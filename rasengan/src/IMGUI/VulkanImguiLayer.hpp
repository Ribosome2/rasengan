#pragma once


// libs
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

// std
#include <stdexcept>


static void check_vk_result(VkResult err) {
  if (err == 0) return;
  fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
  if (err < 0) abort();
}

class VulkanImguiLayer {
 public:
  VulkanImguiLayer(GLFWwindow* window ,  VkRenderPass renderPass, uint32_t imageCount,VkSampleCountFlagBits msaaFlag);
  ~VulkanImguiLayer();

  void NewFrame();

  void Render(VkCommandBuffer commandBuffer);

  void OnGui();

 private:
  VkDescriptorPool descriptorPool;
    GLFWwindow* window;
  void InitTheme();
};

