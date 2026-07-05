#pragma once
#include <glad/glad.h>
#include <string>
#include <unordered_map>
#include "Core/FrostMath.h"
#include "Core/Logger.h"

namespace Frost
{
    class Shader
    {
    public:
        Shader(unsigned int id) : m_ID(id) {}
        ~Shader()
        {
            if (m_ID)
                glDeleteProgram(m_ID);
        }

        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

        void Bind()   const { glUseProgram(m_ID); }
        void Unbind() const { glUseProgram(0); }

        unsigned int ID() const { return m_ID; }

        void SetInt(const std::string& name, int value)
        {
            glUniform1i(GetUniformLocation(name), value);
        }

        void SetFloat(const std::string& name, float value)
        {
            glUniform1f(GetUniformLocation(name), value);
        }

        void SetVec2(const std::string& name, const vec2& v)
        {
            glUniform2f(GetUniformLocation(name), v.x, v.y);
        }

        void SetVec4(const std::string& name, const vec4& v)
        {
            glUniform4f(GetUniformLocation(name), v.r, v.g, v.b, v.a);
        }

        void SetMat4(const std::string& name, const mat4& m)
        {
            glUniformMatrix4fv(
                GetUniformLocation(name), 1, GL_FALSE, m.dataPtr());
        }

    private:
        int GetUniformLocation(const std::string& name)
        {
            auto it = m_UniformCache.find(name);
            if (it != m_UniformCache.end())
                return it->second;

            int location = glGetUniformLocation(m_ID, name.c_str());
            if (location == -1)
                FROST_WARN("Uniform '%s' not found in shader %u",
                    name.c_str(), m_ID);

            m_UniformCache[name] = location;
            return location;
        }

        unsigned int                       m_ID = 0;
        std::unordered_map<std::string, int> m_UniformCache;
    };
}