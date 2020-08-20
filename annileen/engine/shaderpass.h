#pragma once

#include <memory>
#include <engine/shader.h>

namespace annileen
{
    class ShaderPass
    {
        Shader* m_Shader;
        uint64_t m_State;

    public:
        void init(Shader* shader) noexcept;

        Shader* getShader() const noexcept;

        void setState(uint64_t state) { m_State = state; }
        uint64_t getState() { return m_State; }

        ShaderPass();
        ~ShaderPass();
    };
};
