#include "shader.h"

#include <fstream>
#include <streambuf>

namespace annileen
{
    Shader::Shader()
    {
        m_Initialized = false;
    }

    Shader::~Shader()
    {
        destroy();
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

