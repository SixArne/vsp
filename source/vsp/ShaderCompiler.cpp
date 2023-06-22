#include "ShaderCompiler.h"
#include <spdlog/spdlog.h>

namespace vsp
{
    ShaderCompiler::ShaderCompiler()
        : m_LUTExtensions_ShaderType
        {
			{ ".vert", EShLanguage::EShLangVertex },
			{ ".frag", EShLanguage::EShLangFragment },
			{ ".geom", EShLanguage::EShLangGeometry },
			{ ".tesc", EShLanguage::EShLangTessControl },
			{ ".tese", EShLanguage::EShLangTessEvaluation },
			{ ".comp", EShLanguage::EShLangCompute },
            { ".spv" , EShLanguage::EShLangCount }
        },
        m_LUTShaderType_Name
        {
			{ EShLanguage::EShLangVertex,           ".vert" },
			{ EShLanguage::EShLangFragment,         ".frag" },
			{ EShLanguage::EShLangGeometry,         ".geom" },
			{ EShLanguage::EShLangTessControl,      ".tesc" },
			{ EShLanguage::EShLangTessEvaluation,   ".tese" },
			{ EShLanguage::EShLangCompute,          ".comp" },
        },
        m_DefaultResource{ GetDefaultResource() }
	{}

    // Leave here to avoid RAII issues
    ShaderCompiler::~ShaderCompiler()
    {}

    bool ShaderCompiler::SPIR_V_CompileShaderByFile(ShaderFileByteCode& shaderFile)
    {
        bool hasCompiledShader{};

        if (SPIR_V_CompileShader(shaderFile))
        {
            const std::string& parentFolder = shaderFile.filename.parent_path().string();
            const std::string& fileName = shaderFile.filename.string();
            const std::string& shaderType = GetShaderTypeName(shaderFile.shaderType);

            const std::string& output = std::format("{}/{}{}.spv", parentFolder, fileName, shaderType);
            hasCompiledShader = SPIR_V_WriteToFile(output, shaderFile.code);
        }

        return hasCompiledShader;
    }

    TBuiltInResource ShaderCompiler::GetDefaultResource()
    {
        TBuiltInResource Resources{};

        Resources.maxLights = 32;
        Resources.maxClipPlanes = 6;
        Resources.maxTextureUnits = 32;
        Resources.maxTextureCoords = 32;
        Resources.maxVertexAttribs = 64;
        Resources.maxVertexUniformComponents = 4096;
        Resources.maxVaryingFloats = 64;
        Resources.maxVertexTextureImageUnits = 32;
        Resources.maxCombinedTextureImageUnits = 80;
        Resources.maxTextureImageUnits = 32;
        Resources.maxFragmentUniformComponents = 4096;
        Resources.maxDrawBuffers = 32;
        Resources.maxVertexUniformVectors = 128;
        Resources.maxVaryingVectors = 8;
        Resources.maxFragmentUniformVectors = 16;
        Resources.maxVertexOutputVectors = 16;
        Resources.maxFragmentInputVectors = 15;
        Resources.minProgramTexelOffset = -8;
        Resources.maxProgramTexelOffset = 7;
        Resources.maxClipDistances = 8;
        Resources.maxComputeWorkGroupCountX = 65535;
        Resources.maxComputeWorkGroupCountY = 65535;
        Resources.maxComputeWorkGroupCountZ = 65535;
        Resources.maxComputeWorkGroupSizeX = 1024;
        Resources.maxComputeWorkGroupSizeY = 1024;
        Resources.maxComputeWorkGroupSizeZ = 64;
        Resources.maxComputeUniformComponents = 1024;
        Resources.maxComputeTextureImageUnits = 16;
        Resources.maxComputeImageUniforms = 8;
        Resources.maxComputeAtomicCounters = 8;
        Resources.maxComputeAtomicCounterBuffers = 1;
        Resources.maxVaryingComponents = 60;
        Resources.maxVertexOutputComponents = 64;
        Resources.maxGeometryInputComponents = 64;
        Resources.maxGeometryOutputComponents = 128;
        Resources.maxFragmentInputComponents = 128;
        Resources.maxImageUnits = 8;
        Resources.maxCombinedImageUnitsAndFragmentOutputs = 8;
        Resources.maxCombinedShaderOutputResources = 8;
        Resources.maxImageSamples = 0;
        Resources.maxVertexImageUniforms = 0;
        Resources.maxTessControlImageUniforms = 0;
        Resources.maxTessEvaluationImageUniforms = 0;
        Resources.maxGeometryImageUniforms = 0;
        Resources.maxFragmentImageUniforms = 8;
        Resources.maxCombinedImageUniforms = 8;
        Resources.maxGeometryTextureImageUnits = 16;
        Resources.maxGeometryOutputVertices = 256;
        Resources.maxGeometryTotalOutputComponents = 1024;
        Resources.maxGeometryUniformComponents = 1024;
        Resources.maxGeometryVaryingComponents = 64;
        Resources.maxTessControlInputComponents = 128;
        Resources.maxTessControlOutputComponents = 128;
        Resources.maxTessControlTextureImageUnits = 16;
        Resources.maxTessControlUniformComponents = 1024;
        Resources.maxTessControlTotalOutputComponents = 4096;
        Resources.maxTessEvaluationInputComponents = 128;
        Resources.maxTessEvaluationOutputComponents = 128;
        Resources.maxTessEvaluationTextureImageUnits = 16;
        Resources.maxTessEvaluationUniformComponents = 1024;
        Resources.maxTessPatchComponents = 120;
        Resources.maxPatchVertices = 32;
        Resources.maxTessGenLevel = 64;
        Resources.maxViewports = 16;
        Resources.maxVertexAtomicCounters = 0;
        Resources.maxTessControlAtomicCounters = 0;
        Resources.maxTessEvaluationAtomicCounters = 0;
        Resources.maxGeometryAtomicCounters = 0;
        Resources.maxFragmentAtomicCounters = 8;
        Resources.maxCombinedAtomicCounters = 8;
        Resources.maxAtomicCounterBindings = 1;
        Resources.maxVertexAtomicCounterBuffers = 0;
        Resources.maxTessControlAtomicCounterBuffers = 0;
        Resources.maxTessEvaluationAtomicCounterBuffers = 0;
        Resources.maxGeometryAtomicCounterBuffers = 0;
        Resources.maxFragmentAtomicCounterBuffers = 1;
        Resources.maxCombinedAtomicCounterBuffers = 1;
        Resources.maxAtomicCounterBufferSize = 16384;
        Resources.maxTransformFeedbackBuffers = 4;
        Resources.maxTransformFeedbackInterleavedComponents = 64;
        Resources.maxCullDistances = 8;
        Resources.maxCombinedClipAndCullDistances = 8;
        Resources.maxSamples = 4;
        Resources.maxMeshOutputVerticesNV = 256;
        Resources.maxMeshOutputPrimitivesNV = 512;
        Resources.maxMeshWorkGroupSizeX_NV = 32;
        Resources.maxMeshWorkGroupSizeY_NV = 1;
        Resources.maxMeshWorkGroupSizeZ_NV = 1;
        Resources.maxTaskWorkGroupSizeX_NV = 32;
        Resources.maxTaskWorkGroupSizeY_NV = 1;
        Resources.maxTaskWorkGroupSizeZ_NV = 1;
        Resources.maxMeshViewCountNV = 4;

        Resources.limits.nonInductiveForLoops = 1;
        Resources.limits.whileLoops = 1;
        Resources.limits.doWhileLoops = 1;
        Resources.limits.generalUniformIndexing = 1;
        Resources.limits.generalAttributeMatrixVectorIndexing = 1;
        Resources.limits.generalVaryingIndexing = 1;
        Resources.limits.generalSamplerIndexing = 1;
        Resources.limits.generalVariableIndexing = 1;
        Resources.limits.generalConstantMatrixVectorIndexing = 1;

        return Resources;
    }

    bool ShaderCompiler::SPIR_V_CompileShadersByDirectory(const std::string& directory)
    {
        std::vector<ShaderFileByteCode> shaderFiles = FindAllShadersInDirectory(directory);

        for (auto& shaderFile : shaderFiles)
        {
            SPIR_V_CompileShader(shaderFile);

            // CompileShaderByFile should set new name and path
            auto output = std::format("{}/{}{}.spv", shaderFile.filename.parent_path().string(), shaderFile.filename.stem().string(), GetShaderTypeName(shaderFile.shaderType));
            SPIR_V_WriteToFile(output, shaderFile.code);
        }

        // TODO: make sure multithreaded works
        /* std::vector<std::future<bool>> futures{};
        for (auto& shaderFile : shaderFiles)
        {
            futures.emplace_back(std::async(std::launch::async, [&shaderFile]()
                {
                    return SPIR_V_CompileShaderByFile(shaderFile);
                }
            ));
        }

        for (auto& future : futures)
        {
            future.wait();
        }*/

        return true;
    }

    bool ShaderCompiler::SPIR_V_CompileShader(ShaderFileByteCode& shaderFile)
    {
        auto shaderCode = ShaderLoader::ReadShaderFileAsString(shaderFile.filename.string());
        if (!shaderCode.empty())
        {
            // Must initialize glslang before calling any glslang functions
            glslang::InitializeProcess();
            glslang::TShader shader(shaderFile.shaderType);

            std::array<const char*, 1> shaderStrings{};
            shaderStrings[0] = shaderCode.c_str();

            shader.setStrings(shaderStrings.data(), static_cast<int>(shaderStrings.size()));

            int clientInputSemanticsVersion = 100;
            glslang::EShTargetClientVersion vulkanClientVersion = glslang::EShTargetVulkan_1_3;
            glslang::EShTargetLanguageVersion targetVersion = glslang::EShTargetSpv_1_6;
            shader.setEnvTarget(glslang::EshTargetSpv, targetVersion);
            shader.setEnvClient(glslang::EShClientVulkan, vulkanClientVersion);
            shader.setEntryPoint("main");

            const int defaultVersion = 100;

            EShMessages messages = static_cast<EShMessages>(EShMsgSpvRules | EShMsgVulkanRules);

            auto resource = GetDefaultResource();

            if (!shader.parse(&resource, defaultVersion, false, messages)) {
                puts(shader.getInfoLog());
                puts(shader.getInfoDebugLog());
                return false;  // shader failed to compile
            }

            glslang::TProgram program;
            program.addShader(&shader);

            if (!program.link(messages)) {
                puts(program.getInfoLog());
                puts(program.getInfoDebugLog());
                return false;  // program failed to link
            }

            glslang::GlslangToSpv(*program.getIntermediate(shaderFile.shaderType), shaderFile.code);

            return true;
        }

        return false;
    }

	bool ShaderCompiler::SPIR_V_WriteToFile(const std::string& filename, SPIR_V& spirvcode)
	{
        std::ofstream file(filename, std::ios::binary);
        if (file.is_open())
        {
            file.write(reinterpret_cast<const char*>(spirvcode.data()), spirvcode.size() * sizeof(uint32_t));
            file.close();

            SPDLOG_DEBUG("SPIR-V file written as: {}", filename)
            return true;
        }
        else
        {
            SPDLOG_ERROR("SPIR-V file written as: {}", filename)
            return false;
        }
	}

	std::vector<ShaderFileByteCode> ShaderCompiler::FindAllShadersInDirectory(const std::string& directory)
	{
        std::vector<ShaderFileByteCode> shaderFiles{};

		if (std::filesystem::exists(directory))
		{
			for (const auto& entry : std::filesystem::recursive_directory_iterator(directory))
			{
				if (entry.is_regular_file())
				{
					if (auto extension = entry.path().extension(); !extension.empty())
					{
						auto stringExtension = extension.string();

						if (auto it = m_LUTExtensions_ShaderType.find(stringExtension); it != m_LUTExtensions_ShaderType.end())
						{
							auto shaderType = it->second;

                            if (shaderType != EShLangCount)
                            {
                                ShaderFileByteCode shaderFile{};
                                shaderFile.filename = entry.path();
                                shaderFile.shaderType = shaderType;

                                shaderFiles.emplace_back(shaderFile);
                            }
						}
                        else
                        {
                            [[maybe_unused]] std::string fileformats
                            {
                                ".vert, .frag, .geom, .tesc, .tese, .comp"
                            };

                            SPDLOG_WARNING("{} UNKNOWN SHADER FILE", entry.path())
                            SPDLOG_WARNING("Only following extensions are valid {}", fileformats)
                        }
					}
				}
			}
		}

        return shaderFiles;
	}

	std::string ShaderCompiler::GetShaderTypeName(EShLanguage shaderType)
	{
        // Use lookup table to find file extension to use.
		if (auto it = m_LUTShaderType_Name.find(shaderType); it != m_LUTShaderType_Name.end())
		{
			return it->second;
		}

        return std::string{};
	}
}