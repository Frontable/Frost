#pragma once
#include "Shader.h"
#include <memory>
#include <string>

namespace Frost
{
    class ShaderLoader
    {
    public:
        ShaderLoader() = delete;

        static std::shared_ptr<Shader> Create(
            const std::string& vertPath,
            const std::string& fragPath);

    private:
        static unsigned int CreateProgram(
            const std::string& vertPath,
            const std::string& fragPath);

        static unsigned int Compile(
            unsigned int       type,
            const std::string& path);

        static bool CompileSuccess(unsigned int shader);
        static bool LinkSuccess(unsigned int program);
    };
}