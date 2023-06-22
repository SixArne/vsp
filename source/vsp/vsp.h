#ifndef VULKAN_RENDERER_SHADER_COMPILER_H
#define VULKAN_RENDERER_SHADER_COMPILER_H

#include <vector>
#include <string>
#include <filesystem>

#include <vulkan/vulkan.h>

#pragma warning(push)
#pragma warning(disable: 4458)
#pragma warning(disable: 4457)
#include <glslang/Public/ShaderLang.h>
#include <SPIRV/GlslangToSpv.h>
#pragma warning(pop)

namespace vsp
{
    struct ShaderFileByteCode
    {
        std::filesystem::path filename{};
        std::vector<uint32_t> code{};
        EShLanguage shaderType{};
    };

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

    class ShaderCompiler final
    {
    public:
        ShaderCompiler();
        ~ShaderCompiler();

        ShaderCompiler(const ShaderCompiler&) = delete;
        ShaderCompiler& operator=(const ShaderCompiler&) = delete;
        ShaderCompiler(ShaderCompiler&&) = default;
        ShaderCompiler& operator=(ShaderCompiler&&) = default;

        using SPIR_V = std::vector<uint32_t>;

        bool SPIR_V_CompileShadersByDirectory(const std::string& directory);

        static bool SPIR_V_CompileShaderByFile(ShaderFileByteCode& shaderFile);
        static bool SPIR_V_WriteToFile(const std::string& filename, SPIR_V& spirvcode);

    private:
        const std::unordered_map<std::string, EShLanguage> m_LUTExtensions_ShaderType;
        const std::unordered_map<EShLanguage, std::string> m_LUTShaderType_Name;
        const TBuiltInResource m_DefaultResource{};

        std::vector<ShaderFileByteCode> FindAllShadersInDirectory(const std::string& directory, int layers = 0);
        std::string GetShaderTypeName(EShLanguage shaderType);

        static TBuiltInResource GetDefaultResource();
    };

    class ShaderLoader final
    {
    public:
        ShaderLoader(const VkDevice device);
        ~ShaderLoader();

        Shader SPIR_V_LoadShaderByFile(const std::string& filename);
        std::vector<Shader> SPIR_V_LoadShadersByDirectory(const std::string& directory);
        static std::vector<char> ReadShaderFile(const std::string& filename);
        static std::string ReadShaderFileAsString(const std::string& filename);

    private:
        const std::unordered_map<std::string, ShaderType> m_LUTExtensions_ShaderType;

        VkShaderModule CreateShaderModule(const std::vector<char>& code);
        
        const VkDevice m_Device{};
    };
}

#endif
