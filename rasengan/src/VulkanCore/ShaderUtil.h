#pragma once

#include <vector>
#include <fstream>
#include <assert.h>
using namespace std;
class ShaderUtil{
public:
    static void CompileShader(std::string  shaderPath , std::string  outputPath){
        char command[1024];
        sprintf(command,"glslc.exe %s -o %s",shaderPath.c_str(),outputPath.c_str());
        auto result = system(command);
        if(result!=0)
        {
            std::cerr<<"compile shader failed "<<shaderPath<<std::endl;
        }
    }

    static void string_replace(std::string&s1,const std::string&s2,const std::string&s3)
    {
        string::size_type pos=0;
        string::size_type a=s2.size();
        string::size_type b=s3.size();
        while((pos=s1.find(s2,pos))!=string::npos)
        {
            s1.replace(pos,a,s3);
            pos+=b;
        }
    }

    static std::vector<char> ReadFile(const std::string & sourceShaderPath)
    {
        auto spv_file_path = std::string(sourceShaderPath);
        string_replace(spv_file_path,"shaders/","spv_files/");
        spv_file_path+=".spv";
        ShaderUtil::CompileShader(sourceShaderPath, spv_file_path);

        std::ifstream file(spv_file_path, std::ios::ate | std::ios::binary);
        //std::ios::ate flag means we are reading at the end of the file ,because we want to use the read position
        //to determine the size of file and allocate a buffer
        if (!file.is_open())
        {
            throw std::runtime_error("failed to open file! "+spv_file_path);
        }

        size_t fileSize = (size_t)file.tellg();
        std::vector<char> buffer(fileSize);
        file.seekg(0);  //got the size ,seek back to beginning
        file.read(buffer.data(), fileSize);
        file.close();
        return buffer;
    }

    static VkShaderModule CreateShaderModule(VkDevice &  device, const std::vector<char> &code) {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode =reinterpret_cast<const uint32_t *>(code.data());
        assert(device!=VK_NULL_HANDLE);
        VkShaderModule shaderModule;
        if(vkCreateShaderModule(device,&createInfo, nullptr,&shaderModule)!=VK_SUCCESS){
            throw std::runtime_error("failed to create shader modules");
        }
        return  shaderModule;
    }

    static VkPipelineShaderStageCreateInfo GetShaderStageCreateInfo(VkShaderModule & vertShaderModule, VkShaderStageFlagBits stageFlag){
        VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = stageFlag;
        vertShaderStageInfo.module = vertShaderModule;
        vertShaderStageInfo.pName = "main";
        return vertShaderStageInfo;
    }


};

