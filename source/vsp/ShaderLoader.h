#ifndef VSP_SHADER_LOADER_H
#define VSP_SHADER_LOADER_H

#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <unordered_map>

#include <vulkan/vulkan.h>

namespace vsp
{
    enum class ShaderType
    {
        Vertex,
        Fragment,
        Geometry,
        Compute,
        TessellationControl,
        TessellationEvaluation
    };

    struct Shader
    {
        VkShaderModule shaderModule{};
        ShaderType shaderType{};
    };

    class ShaderLoader final
    {
    public:
        ShaderLoader(const VkDevice device);
        ~ShaderLoader();

        ShaderLoader(const ShaderLoader&) = delete;
        ShaderLoader& operator=(const ShaderLoader&) = delete;
        ShaderLoader(ShaderLoader&&) = delete;
        ShaderLoader& operator=(ShaderLoader&&) = delete;

        Shader SPIR_V_LoadShaderByFile(const std::string& filename);
        std::vector<Shader> SPIR_V_LoadShadersByDirectory(const std::string& directory);
        static std::vector<char> ReadShaderFile(const std::string& filename);
        static std::string ReadShaderFileAsString(const std::string& filename);

    private:
        const VkDevice m_Device{};
        const std::unordered_map<std::string, ShaderType> m_LUTExtensions_ShaderType;

        VkShaderModule CreateShaderModule(const std::vector<char>& code);
    };
}

#endif // !VSP_SHADER_LOADER_H