#include <engine/shaderpass.h>

namespace annileen
{
    void ShaderPass::init(std::shared_ptr<Shader> shader) noexcept
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

    std::shared_ptr<Shader> ShaderPass::getShader() const noexcept
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

