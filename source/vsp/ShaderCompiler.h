#ifndef VSP_SHADER_COMPILER_H
#define VSP_SHADER_COMPILER_H

#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <unordered_map>

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

    class ShaderCompiler final
    {
    public:
        ShaderCompiler();
        ~ShaderCompiler();

        ShaderCompiler(const ShaderCompiler&) = delete;
        ShaderCompiler& operator=(const ShaderCompiler&) = delete;
        ShaderCompiler(ShaderCompiler&&) = delete;
        ShaderCompiler& operator=(ShaderCompiler&&) = delete;

        using SPIR_V = std::vector<uint32_t>;

        bool SPIR_V_CompileShadersByDirectory(const std::string& directory);
        bool SPIR_V_CompileShaderByFile(ShaderFileByteCode& shaderFile);

    private:
        // Loopup tables for shader types.
        const std::unordered_map<std::string, EShLanguage> m_LUTExtensions_ShaderType;
        const std::unordered_map<EShLanguage, std::string> m_LUTShaderType_Name;
        
        // Default limits and settings for shader files
        const TBuiltInResource m_DefaultResource{};

        bool SPIR_V_CompileShader(ShaderFileByteCode& shaderFile);
        bool SPIR_V_WriteToFile(const std::string& filename, SPIR_V& spirvcode);

        std::vector<ShaderFileByteCode> FindAllShadersInDirectory(const std::string& directory);
        std::string GetShaderTypeName(EShLanguage shaderType);

        static TBuiltInResource GetDefaultResource();
    };
}

#endif // !VSP_SHADER_COMPILER_H