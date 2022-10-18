#pragma once

#include "EngineCore/Scene.h"
#include "VulkanCore/VulkanPipeline.h"
#include "EngineCore/Material.h"
#include "EngineCore/MeshRenderer.h"
#include "EngineCore/Geometry/Cube.h"
#include "VulkanCore/VulkanTexture.h"
#include "EngineCore/ModelLoader.h"

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

        //floor init
        auto floorGo = std::make_shared<GameObject>();
        floorGo->transform.scale=glm::vec3(10,0.2,10);

        floorGo->meshRenderer = std::make_shared<MeshRenderer>();
        floorGo->meshRenderer->mesh = std::make_shared<Cube>();
		floorGo->AddComponent(floorGo->meshRenderer.get());
        auto floorMaterial= std::make_shared<Material>();
        floorMaterial->shader = testShader;
        floorMaterial->pipeline = pipeline;
        floorMaterial->CreateDescriptorSets(floorMaterial->shader->descriptorSetLayout);
        floorGo->meshRenderer->material =floorMaterial;
        gameObjects.push_back(floorGo);

        for (int i = 0; i < 4; ++i) {
            auto quadGo = std::make_shared<GameObject>();
            quadGo->AutoRotate=true;
            quadGo->transform.scale = glm::vec3{.5};
            quadGo->transform.position = glm::vec3{0.8*i-2, 0.8, -1.0};
            auto meshRenderer = std::make_shared<MeshRenderer>();
            std::shared_ptr<Material> testMaterial = std::make_shared<Material>();
            meshRenderer->material = testMaterial;
            testMaterial->name = "TestMaterial";

            VulkanTexture * texturePt = nullptr;
            quadGo->transform.eulerAngles.z =180;
            if(i==2)
            {
                meshRenderer->mesh = std::make_shared<Quad>();
                texturePt = new VulkanTexture("Assets/Textures/texture.jpg");
            }else if(i==3){
                meshRenderer->mesh = std::make_shared<Mesh>("Assets/Models/vikingRoom/viking_room.obj");;
                texturePt = new VulkanTexture("Assets/Models/vikingRoom/viking_room.png");
                quadGo->transform.eulerAngles.z =90;
            }
            else{
                meshRenderer->mesh = std::make_shared<Cube>();
                texturePt = new VulkanTexture("Assets/Textures/KaiSa.png");
            }
            testMaterial->mainTexture = texturePt;
            textures.push_back(texturePt);
            testMaterial->shader = testShaderWithSampler;
            testMaterial->pipeline = texturePipeline;
            testMaterial->CreateDescriptorSets(testMaterial->shader->descriptorSetLayout);
            quadGo->meshRenderer = meshRenderer;
			quadGo->AddComponent(meshRenderer.get());
            gameObjects.push_back(quadGo);
        }
    }

    virtual ~DemoScene() {
        delete testShader;
        delete pipeline;
        delete testShaderWithSampler;
        delete texturePipeline;
        for (auto pTex:textures) {
            delete pTex;
        }
    }

private:
    VulkanShader *testShader;
    VulkanShader *testShaderWithSampler;
    VulkanPipeline *pipeline;
    VulkanPipeline *texturePipeline;
    std::vector<VulkanTexture *> textures;
};

