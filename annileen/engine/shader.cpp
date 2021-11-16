#include "shader.h"

#include <fstream>
#include <streambuf>

namespace annileen
{
    Shader::Shader()
    {

    }

    Shader::~Shader()
    {
        destroy();
    }

    void Shader::init(bgfx::ProgramHandle handle)
    {
        m_ProgramHandle = handle;
    }

    void Shader::destroy()
    {
        bgfx::destroy(m_ProgramHandle);
        m_AvailableUniforms.clear();
    }
}

