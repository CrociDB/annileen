#include <engine/renderview.h>

namespace annileen
{
    std::map<size_t, RenderView*> RenderView::m_ViewIds;

    RenderView::RenderView()
    {
    }

    RenderView::RenderView(bgfx::ViewId viewId, const char* viewName)
    {
        m_Id = viewId;
        m_Name = viewName;
    }

    void RenderView::addRenderView(size_t id, RenderView* renderView)
    {
        // TODO: add asserts, etc.

        m_ViewIds[id] = renderView;
        bgfx::setViewName(renderView->m_Id, renderView->m_Name);
    }

    RenderView* RenderView::getRenderView(size_t id)
    {
        auto renderViewIt = m_ViewIds.find(id);
        if (renderViewIt != m_ViewIds.end())
        {
            return renderViewIt->second;
        }
        return nullptr;
    }


    RenderView::~RenderView()
    {
    }
}

