module;

#include <memory>
#include <vector>
#include <bgfx/defines.h>

export module shaderpass;

import shader;

export namespace annileen
{
    class ShaderPass
    {
        Shader* m_Shader;
        uint64_t m_State;

    public:
        void init(Shader* shader) noexcept;

        Shader* getShader() const noexcept;

        inline void setState(uint64_t state) { m_State = state; }
        inline uint64_t getState() { return m_State; }
        inline const std::vector<ShaderAvailableUniform>& getShaderAvailableUniforms()
        {
            return m_Shader->getAvailableShaders();
        }

        ShaderPass();
        ~ShaderPass();
    };
};


namespace annileen
{
    void ShaderPass::init(Shader* shader) noexcept
    {
        m_Shader = shader;

        m_State = BGFX_STATE_WRITE_RGB
            | BGFX_STATE_WRITE_A
            | BGFX_STATE_WRITE_Z
            | BGFX_STATE_DEPTH_TEST_LESS
            | BGFX_STATE_CULL_CCW
            | BGFX_STATE_MSAA
            | UINT64_C(0); 
    }

    Shader* ShaderPass::getShader() const noexcept
    {
        return m_Shader;
    }

    ShaderPass::ShaderPass()
    {
    }

    ShaderPass::~ShaderPass()
    {
    }
}

