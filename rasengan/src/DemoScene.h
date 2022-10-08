#pragma once

#include "EngineCore/Scene.h"
#include "VulkanCore/VulkanPipeline.h"
#include "EngineCore/Material.h"
#include "EngineCore/MeshRenderer.h"

class DemoScene : public Scene {
public:
    DemoScene() {
        testShader = new VulkanShader("shaders/vertexWithUniformBuffer.vert", "shaders/simpleColor.frag");
        pipeline = new VulkanPipeline(*testShader);

        for (int i = 0; i < 5; ++i) {
            auto meshRenderer = std::make_shared<MeshRenderer>();
            std::shared_ptr<Material> testMaterial = std::make_shared<Material>();
            meshRenderer->material = testMaterial;
            testMaterial->name = "TestMaterial";
            testMaterial->shader = testShader;
            testMaterial->pipeline = pipeline;
            testMaterial->CreateDescriptorSets(testShader->descriptorSetLayout);
            auto quadGo = std::make_shared<GameObject>();
            quadGo->transform.gameObject = quadGo.get();
            quadGo->transform.scale = glm::vec3{0.2};
            quadGo->transform.position = glm::vec3{0.2, 0.3 * i, 0.0};
            if (i == 0) //first one as floor
            {
                quadGo->transform.eulerAngles = glm::vec3{-90,0,0};
                quadGo->transform.scale = glm::vec3{2.0};
                quadGo->transform.position.y = 0;
            }

            quadGo->meshRenderer = meshRenderer;
            meshRenderer->transform = &quadGo->transform;
            gameObjects.push_back(quadGo);
        }
    }

    virtual ~DemoScene() {
        delete testShader;
        delete pipeline;
    }

private:
    VulkanShader *testShader;
    VulkanPipeline *pipeline;
};

