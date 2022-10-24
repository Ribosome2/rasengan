#pragma once

#include "spirv_reflect.h"

class VulkanSpirvReflection {
public:
    static void SpirvReflectExample(const void *spirv_code, size_t spirv_nbytes);
    static void PrintInputAndOutput(SpvReflectShaderModule module);
};
