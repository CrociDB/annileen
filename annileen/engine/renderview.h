#pragma once

#include<bgfx/bgfx.h>
#include<unordered_map>
#include<string_view>
#include<list>

namespace annileen
{
    class RenderView
    {
        bgfx::ViewId m_Id;
        std::string_view m_Name;

        static std::unordered_map<size_t, RenderView*> m_ViewIds;
        static std::list<size_t> m_RenderViewIds;

    public:
        constexpr static uint8_t maximumNumberOfRenderViews = 254;

        enum ReservedIndices : size_t
        {
            Shadow = 0,
            Scene,
            Skybox,
            PostProcessing            
        };

        bgfx::ViewId getViewId() const noexcept { return m_Id; }

        static bool addRenderView(const size_t id, const char* name);
        static RenderView* getRenderView(const size_t id);
        static bool hasRenderView(const size_t id);
        static bool insertOrMoveRenderViewAfter(const size_t id, const char* name, const size_t afterThisId);
        static bool insertOrMoveRenderViewBefore(const size_t id, const char* name, const size_t beforeThisId);
        static bool removeRenderView(const size_t id);

        RenderView();
        RenderView(bgfx::ViewId viewId, std::string_view viewName);
        ~RenderView();
    private:
        friend class Renderer;
        static bool addReservedRenderView(const size_t id, const char* name);
        static bool insertOrMoveReservedRenderViewAfter(const size_t id, const char* name, const size_t afterThisId);
        static bool insertOrMoveReservedRenderViewBefore(const size_t id, const char* name, const size_t beforeThisId);
        static bool removeReservedRenderView(const size_t id);
        static void redefineViewIds();

    };
};
