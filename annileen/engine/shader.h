#ifndef _SHADER_H_
#define _SHADER_H_

#include <iostream>
#include <string>
#include <vector>

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

        std::vector<ShaderAvailableUniform> m_AvailableUniforms;

    public:
        void init(bgfx::ProgramHandle handle);
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


#endif
