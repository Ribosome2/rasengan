#include "VulkanSpirvReflection.h"
#include "spirv_reflect.h"
#include "VulkanSpirvCommon.h"

#include <cstring>
#include <fstream>
#include <sstream>
#include <cassert>
#include <iostream>

void VulkanSpirvReflection::SpirvReflectExample(const void *spirv_code, size_t spirv_nbytes) {
    // Generate reflection data for a shader
    std::cout << "SpvReflectShaderModule:" << "\n";
    SpvReflectShaderModule module;
    SpvReflectResult result = spvReflectCreateShaderModule(spirv_nbytes, spirv_code, &module);
    assert(result == SPV_REFLECT_RESULT_SUCCESS);
    PrintModuleInfo(std::cout, module);

    std::cout << "\n\n";
    uint32_t count = 0;
    result = spvReflectEnumerateDescriptorSets(&module, &count, NULL);
    assert(result == SPV_REFLECT_RESULT_SUCCESS);

    std::vector<SpvReflectDescriptorSet *> sets(count);
    result = spvReflectEnumerateDescriptorSets(&module, &count, sets.data());
    assert(result == SPV_REFLECT_RESULT_SUCCESS);

    std::cout << "Descriptor sets:" << "\n";
    // Log the descriptor set contents to stdout
    const char *t = "  ";
    const char *tt = "    ";
    for (size_t index = 0; index < sets.size(); ++index) {
        auto p_set = sets[index];

        // descriptor sets can also be retrieved directly from the module, by set index
        auto p_set2 = spvReflectGetDescriptorSet(&module, p_set->set, &result);
        assert(result == SPV_REFLECT_RESULT_SUCCESS);
        assert(p_set == p_set2);
        (void) p_set2;

        std::cout << t << index << ":" << "\n";
        PrintDescriptorSet(std::cout, *p_set, tt);
        std::cout << "\n\n";
    }


    PrintInputAndOutput(module);


    // Output variables, descriptor bindings, descriptor sets, and push constants
    // can be enumerated and extracted using a similar mechanism.

    // Destroy the reflection data when no longer required.
    spvReflectDestroyShaderModule(&module);
}

void VulkanSpirvReflection::PrintInputAndOutput(SpvReflectShaderModule module) {
    uint32_t count = 0;
    SpvReflectResult result = spvReflectEnumerateInputVariables(&module, &count, NULL);
    assert(result == SPV_REFLECT_RESULT_SUCCESS);

    std::vector<SpvReflectInterfaceVariable *> input_vars(count);
    result = spvReflectEnumerateInputVariables(&module, &count, input_vars.data());
    assert(result == SPV_REFLECT_RESULT_SUCCESS);

    count = 0;
    result = spvReflectEnumerateOutputVariables(&module, &count, NULL);
    assert(result == SPV_REFLECT_RESULT_SUCCESS);

    std::vector<SpvReflectInterfaceVariable *> output_vars(count);
    result = spvReflectEnumerateOutputVariables(&module, &count, output_vars.data());
    assert(result == SPV_REFLECT_RESULT_SUCCESS);

    // Log the interface variables to stdout
    const char *t = "  ";
    const char *tt = "    ";
    std::cout << "\n\n";
    std::cout << "Input variables:" << "\n";
    for (size_t index = 0; index < input_vars.size(); ++index) {
        auto p_var = input_vars[index];

        // input variables can also be retrieved directly from the module, by location
        // (unless the location is (uint32_t)-1, as is the case with built-in inputs)
        auto p_var2 = spvReflectGetInputVariableByLocation(&module, p_var->location, &result);
        if (p_var->location == UINT32_MAX) {
            assert(result == SPV_REFLECT_RESULT_ERROR_ELEMENT_NOT_FOUND);
            assert(p_var2 == nullptr);
        } else {
            assert(result == SPV_REFLECT_RESULT_SUCCESS);
            assert(p_var == p_var2);
        }
        (void) p_var2;

        // input variables can also be retrieved directly from the module, by semantic (if present)
        p_var2 = spvReflectGetInputVariableBySemantic(&module, p_var->semantic, &result);
        if (!p_var->semantic) {
            assert(result == SPV_REFLECT_RESULT_ERROR_NULL_POINTER);
            assert(p_var2 == nullptr);
        } else if (p_var->semantic[0] != '\0') {
            assert(result == SPV_REFLECT_RESULT_ERROR_ELEMENT_NOT_FOUND);
            assert(p_var2 == nullptr);
        } else {
            assert(result == SPV_REFLECT_RESULT_SUCCESS);
            assert(p_var == p_var2);
        }
        (void) p_var2;

        std::cout << t << index << ":" << "\n";
        PrintInterfaceVariable(std::cout, module.source_language, *p_var, tt);
        std::cout << "\n\n";
    }

    std::cout << "Output variables:" << "\n";
    for (size_t index = 0; index < output_vars.size(); ++index) {
        auto p_var = output_vars[index];

        // output variables can also be retrieved directly from the module, by location
        // (unless the location is (uint32_t)-1, as is the case with built-in outputs)
        auto p_var2 = spvReflectGetOutputVariableByLocation(&module, p_var->location, &result);
        if (p_var->location == UINT32_MAX) {
            assert(result == SPV_REFLECT_RESULT_ERROR_ELEMENT_NOT_FOUND);
            assert(p_var2 == nullptr);
        } else {
            assert(result == SPV_REFLECT_RESULT_SUCCESS);
            assert(p_var == p_var2);
        }
        (void) p_var2;

        // output variables can also be retrieved directly from the module, by semantic (if present)
        p_var2 = spvReflectGetOutputVariableBySemantic(&module, p_var->semantic, &result);
        if (!p_var->semantic) {
            assert(result == SPV_REFLECT_RESULT_ERROR_NULL_POINTER);
            assert(p_var2 == nullptr);
        } else if (p_var->semantic[0] != '\0') {
            assert(result == SPV_REFLECT_RESULT_ERROR_ELEMENT_NOT_FOUND);
            assert(p_var2 == nullptr);
        } else {
            assert(result == SPV_REFLECT_RESULT_SUCCESS);
            assert(p_var == p_var2);
        }
        (void) p_var2;

        std::cout << t << index << ":" << "\n";
        PrintInterfaceVariable(std::cout, module.source_language, *p_var, tt);
        std::cout << "\n\n";
    }
}
