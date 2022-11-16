#pragma once


#include <vulkan/vulkan.h>
#include <iostream>
#include "VulkanVertexBuffer.h"
#include "VulkanIndexBuffer.h"
#include "EngineCore/GameObject.h"
#include "EngineCore/MeshRenderer.h"
#include "EngineCore/Material.h"
#include "memory"
#include "EngineCore/SceneViewCameraController.h"
#include "EngineCore/Camera.h"

class RenderContext {
public:
    MeshRenderer *meshRenderer;
    Material *material;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
};

class VulkanRenderer {
public:
    VulkanRenderer();

    ~VulkanRenderer();

    void Init();

    void Update();

    void BeginRenderPass(uint32_t imageIndex);

    void EndRenderPass();

    void BeginFrame();

    void DrawFrame(std::vector<std::shared_ptr<GameObject>> gameObjects);

    void EndFrame();

    void RecordCommandBuffer(VkCommandBuffer &commandBuffer, uint32_t imageIndex);

    VkDescriptorPool &GetDescriptorPool() { return descriptorPool; };
private:
    void createDescriptorPool();

    void updateCameraMatrix();

public:
    uint32_t imageIndex;
    RenderContext RenderContext;
    Camera camera;
    SceneViewCameraController cameraController{&camera};
private:
    VkDescriptorPool descriptorPool = VK_NULL_HANDLE;
    static bool useWireFramePipeline;

};
