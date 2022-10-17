#pragma once
#include <string>
#include <vector>
#include "VulkanCore/VulkanVertex.h"

class ModelLoader {
public:
    static void LoadModel(std::string path,
                          std::vector<VulkanVertex> & vertices,
                          std::vector<uint16_t>&  indices
    );
};
