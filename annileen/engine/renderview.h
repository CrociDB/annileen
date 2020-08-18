#pragma once

#include<bgfx/bgfx.h>
#include<map>
#include<string>

namespace annileen
{
    class RenderView
    {
        bgfx::ViewId m_Id;
        const char* m_Name;

        static std::map<size_t, RenderView*> m_ViewIds;

    public:
        enum ReservedIndices : size_t
        {
            Shadow = 0,
            Scene,
            Skybox
        };

        bgfx::ViewId getId() const noexcept { return m_Id; }

        static void addRenderView(size_t id, RenderView* renderView);
        static RenderView* getRenderView(size_t id);

        RenderView();
        RenderView(bgfx::ViewId viewId, const char* viewName);
        ~RenderView();
    };
};
