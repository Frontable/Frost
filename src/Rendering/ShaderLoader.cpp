#include "ShaderLoader.h"
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include "Core/Logger.h"

namespace Frost
{
    std::shared_ptr<Shader> ShaderLoader::Create(
        const std::string& vertPath,
        const std::string& fragPath)
    {
        unsigned int program = CreateProgram(vertPath, fragPath);
        if (!program) return nullptr;
        return std::make_shared<Shader>(program);
    }

    unsigned int ShaderLoader::CreateProgram(
        const std::string& vertPath,
        const std::string& fragPath)
    {
        unsigned int program = glCreateProgram();
        unsigned int vertex = Compile(GL_VERTEX_SHADER, vertPath);
        unsigned int fragment = Compile(GL_FRAGMENT_SHADER, fragPath);

        if (!vertex || !fragment)
        {
            glDeleteProgram(program);
            return 0;
        }

        glAttachShader(program, vertex);
        glAttachShader(program, fragment);
        glLinkProgram(program);

        if (!LinkSuccess(program)) return 0;

        glDeleteShader(vertex);
        glDeleteShader(fragment);

        FROST_LOG("Shader program created: %u", program);
        return program;
    }

    unsigned int ShaderLoader::Compile(
        unsigned int       type,
        const std::string& path)
    {
        std::ifstream file(path);
        if (!file.is_open())
        {
            FROST_ERROR("Failed to open shader: %s", path.c_str());
            return 0;
        }

        std::stringstream ss;
        ss << file.rdbuf();
        std::string src = ss.str();
        const char* srcPtr = src.c_str();

        unsigned int shader = glCreateShader(type);
        glShaderSource(shader, 1, &srcPtr, nullptr);
        glCompileShader(shader);

        if (!CompileSuccess(shader)) return 0;
        return shader;
    }

    bool ShaderLoader::CompileSuccess(unsigned int shader)
    {
        int success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char log[512];
            glGetShaderInfoLog(shader, 512, nullptr, log);
            FROST_ERROR("Shader compile error: %s", log);
            glDeleteShader(shader);
            return false;
        }
        return true;
    }

    bool ShaderLoader::LinkSuccess(unsigned int program)
    {
        int success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success)
        {
            char log[512];
            glGetProgramInfoLog(program, 512, nullptr, log);
            FROST_ERROR("Shader link error: %s", log);
            glDeleteProgram(program);
            return false;
        }
        return true;
    }
}