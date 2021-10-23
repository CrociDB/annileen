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
        bgfx::destroy(m_ProgramHandle);
    }

    void Shader::init(bgfx::ProgramHandle handle)
    {
        m_ProgramHandle = handle;
    }
}

