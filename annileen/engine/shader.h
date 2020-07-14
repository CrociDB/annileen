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
        void use();
    
        void setBool(const std::string& name, bool value);
        void setInt(const std::string& name, int value);
        void setFloat(const std::string& name, float value);
        void setVec2(const std::string& name, const glm::vec2& value);
        void setVec3(const std::string& name, const glm::vec3& value);
        void setVec4(const std::string& name, const glm::vec4& value);
        void setMat4(const std::string& name, const glm::mat4& value);

        Shader();
        ~Shader();
    };
};


#endif
