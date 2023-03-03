module;

#include <fstream>
#include <streambuf>
#include <iostream>
#include <string>
#include <vector>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <bgfx/bgfx.h>

export module anni.shader;

import anni.asset;

export namespace annileen
{
    class Shader : public AssetObject
    {
    private:
        bool m_Initialized;

        bgfx::ProgramHandle m_ProgramHandle;

        std::vector<ShaderAvailableUniform> m_AvailableUniforms;

    public:
        void setHandle(bgfx::ProgramHandle handle);
        void destroy();

        inline void setAvailableShaders(std::vector<ShaderAvailableUniform> availableUniform)
        {
            m_AvailableUniforms = availableUniform;
        }

        inline const std::vector<ShaderAvailableUniform>& getAvailableShaders()
        {
            return m_AvailableUniforms;
        }

        inline bgfx::ProgramHandle getProgram() { return m_ProgramHandle; }

        Shader();
        ~Shader();
    };
};

module :private;

namespace annileen
{
    Shader::Shader()
    {
        m_Initialized = false;
    }

    Shader::~Shader()
    {
        destroy();

        // TODO: remove
        std::cout << "Shader destroyed." << std::endl;
    }

    void Shader::setHandle(bgfx::ProgramHandle handle)
    {
        m_ProgramHandle = handle;
        m_Initialized = true;
    }

    void Shader::destroy()
    {
        if (m_Initialized)
        {
            bgfx::destroy(m_ProgramHandle);
            m_AvailableUniforms.clear();
            m_Initialized = false;
        }
    }
}

