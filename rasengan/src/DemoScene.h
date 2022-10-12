#pragma once

#include "EngineCore/Scene.h"
#include "VulkanCore/VulkanPipeline.h"
#include "EngineCore/Material.h"
#include "EngineCore/MeshRenderer.h"
#include "EngineCore/Geometry/Cube.h"
#include "VulkanCore/VulkanTexture.h"

class DemoScene : public Scene {
public:
    DemoScene() {
        testShader = new VulkanShader("shaders/vertexWithUniformBuffer.vert", "shaders/simpleColor.frag");

        //TODO: create binding by shader content automatically
        testShader->AddDescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,VK_SHADER_STAGE_VERTEX_BIT);
        testShader->CreateDescriptorSetLayout();

        testShaderWithSampler = new VulkanShader("shaders/simpleTexture.vert", "shaders/simpleTexture.frag");
        testShaderWithSampler->AddDescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,VK_SHADER_STAGE_VERTEX_BIT);
        testShaderWithSampler->AddDescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,VK_SHADER_STAGE_FRAGMENT_BIT);
        testShaderWithSampler->CreateDescriptorSetLayout();

        pipeline = new VulkanPipeline(*testShader);
        texturePipeline = new VulkanPipeline(*testShaderWithSampler);

        auto testTexture = new VulkanTexture("Assets/Textures/texture.jpg");
        for (int i = 0; i < 2; ++i) {
            auto meshRenderer = std::make_shared<MeshRenderer>();
			if(i!=0)
			{
				meshRenderer->mesh = std::make_shared<Quad>();
			}else{

				meshRenderer->mesh = std::make_shared<Cube>();
			}
            std::shared_ptr<Material> testMaterial = std::make_shared<Material>();
            meshRenderer->material = testMaterial;
            testMaterial->name = "TestMaterial";
            auto quadGo = std::make_shared<GameObject>();
            quadGo->transform.gameObject = quadGo.get();
            quadGo->transform.scale = glm::vec3{1.2};
            quadGo->transform.position = glm::vec3{0.2, 0.3 * i, 0.0};
            if(i%2==0)
            {
                testMaterial->shader = testShader;
                testMaterial->pipeline = pipeline;

            }else{
                testMaterial->shader = testShaderWithSampler;
                testMaterial->pipeline = texturePipeline;
                testMaterial->mainTexture = testTexture;
                quadGo->transform.eulerAngles.z =180;
            }

            testMaterial->CreateDescriptorSets(testMaterial->shader->descriptorSetLayout);

            if (i == 0) //first one as floor
            {
                quadGo->transform.scale = glm::vec3{1.0,0.1,1.0};
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
        delete testShaderWithSampler;
    }

private:
    VulkanShader *testShader;
    VulkanShader *testShaderWithSampler;
    VulkanPipeline *pipeline;
    VulkanPipeline *texturePipeline;
};

