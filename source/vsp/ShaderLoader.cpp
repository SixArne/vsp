#include "ShaderLoader.h"
#include <spdlog/spdlog.h>

namespace vsp
{
    std::vector<char> ShaderLoader::ReadShaderFile(const std::string& filename)
    {
        std::vector<char> output{};
        if (std::ifstream file{filename, std::ios::binary})
        {
            output.assign(
                std::istreambuf_iterator<char>(file),
                std::istreambuf_iterator<char>()
            );
        }
        else
        {
            SPDLOG_ERROR("Failed to read shader file: {}", filename);
        }

        return output;
    }

    std::string ShaderLoader::ReadShaderFileAsString(const std::string& filename)
    {
        std::string output{};
        if (std::ifstream file{filename, std::ios::binary})
        {
            output.assign(
                std::istreambuf_iterator<char>(file),
                std::istreambuf_iterator<char>()
            );
        }
        else
        {
            SPDLOG_ERROR("Failed to read shader file: {}", filename);
            return output;
        }

        return output;
    }

    Shader ShaderLoader::SPIR_V_LoadShaderByFile(const std::string& filename)
	{
        // Shader data to return
        Shader shader{};
        
        // Raw shader code
        auto shaderFile = ReadShaderFile(filename);
        
        // We need to extract the name of the shader from the filename
        std::filesystem::path pathToFile = filename;
        std::filesystem::path shaderName = pathToFile.stem();

        // Now we extract the type of shader by it's sub-extension
        for (const auto& pair: m_LUTExtensions_ShaderType)
        {
            if (shaderName.string().find(pair.first) != std::string::npos)
            {
                shader.shaderType = pair.second;
                break;
            }
        }

        // Create the shader module
        shader.shaderModule = CreateShaderModule(shaderFile);

        return shader;
	}

    std::vector<Shader> ShaderLoader::SPIR_V_LoadShadersByDirectory(const std::string& directory)
    {
        std::vector<Shader> shaderFiles{};

		if (std::filesystem::exists(directory))
		{
			for (const auto& entry : std::filesystem::recursive_directory_iterator(directory))
			{
				if (entry.is_regular_file())
				{
					if (auto extension = entry.path().extension(); !extension.empty())
					{
						auto stringExtension = extension.string();

						if (stringExtension.find(".spv") != std::string::npos)
						{
                            // rework LoadShadersbyFile to use std::filesystem::path instead
							Shader shader = SPIR_V_LoadShaderByFile(entry.path().string());
                            shaderFiles.emplace_back(shader);
						}
					}
				}
			}
		}

        return shaderFiles;
	}

	VkShaderModule ShaderLoader::CreateShaderModule(const std::vector<char>& code)
	{
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        VkShaderModule shaderModule{};
        if (vkCreateShaderModule(m_Device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
        {
            //L_ERROR("Failed to create shader module");
        }

        return shaderModule;
	}

	ShaderLoader::ShaderLoader(const VkDevice device)
        : m_Device{device},
        m_LUTExtensions_ShaderType
        {
			{ ".vert", ShaderType::Vertex },
            { ".frag", ShaderType::Fragment },
            { ".geom", ShaderType::Geometry },
            { ".tesc", ShaderType::TessellationControl },
            { ".tese", ShaderType::TessellationEvaluation },
            { ".comp", ShaderType::Compute }
        }
	{}

	ShaderLoader::~ShaderLoader()
	{}
}