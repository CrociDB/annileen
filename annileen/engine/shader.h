#ifndef _SHADER_H_
#define _SHADER_H_

#include <iostream>
#include <string>

#include "asset.h"

#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <bgfx/bgfx.h>

namespace annileen
{
    class Shader : public AssetObject
    {
    private:
        bgfx::ProgramHandle m_ProgramHandle;

    public:
        void init(bgfx::ProgramHandle handle);

        bgfx::ProgramHandle getProgram() { return m_ProgramHandle; }

        Shader();
        ~Shader();
    };
};


#endif
