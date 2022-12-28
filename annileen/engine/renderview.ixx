module;

#include <algorithm>
#include<bgfx/bgfx.h>
#include<unordered_map>
#include<string_view>
#include<list>
#include"engine/core/logger.h"

export module renderview;


export namespace annileen
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
            UI,
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

namespace annileen
{
    constexpr uint8_t RenderView::maximumNumberOfRenderViews;
    std::unordered_map<size_t, RenderView*> RenderView::m_ViewIds;
    std::list<size_t> RenderView::m_RenderViewIds;

    RenderView::RenderView() : m_Id(0), m_Name("")
    {
    }

    RenderView::RenderView(bgfx::ViewId viewId, std::string_view viewName)
    {
        m_Id = viewId;
        m_Name = viewName;
    }

    RenderView::~RenderView()
    {
    }

    void RenderView::redefineViewIds()
    {
        bgfx::ViewId viewId = 0;
        for (auto renderViewId : m_RenderViewIds)
        {
            m_ViewIds[renderViewId]->m_Id = viewId++;
        }
    }

    bool RenderView::addRenderView(const size_t id, const char* name)
    {
        if (id <= PostProcessing)
        {
            ANNILEEN_LOGF_ERROR(LoggingChannel::Renderer, "RenderView cannot be added, because RenderView id {0} is reserved by the engine.", id);
            return false;
        }

        return addReservedRenderView(id, name);
    }

    bool RenderView::addReservedRenderView(const size_t id, const char* name)
    {
        if (m_ViewIds.size() == maximumNumberOfRenderViews)
        {
            ANNILEEN_LOG_ERROR(LoggingChannel::Renderer, "Maximum number of RenderViews was reached already");
            return false;
        }

        auto renderViewIt = m_ViewIds.find(id);
        if (renderViewIt != m_ViewIds.end())
        {
            ANNILEEN_LOG_ERROR(LoggingChannel::Renderer, "RenderView cannot be added, because there is a RenderView with the same id already.");
            return false;
        }

        RenderView *renderView = new RenderView(static_cast<bgfx::ViewId>(m_ViewIds.size()), std::string_view(name));

        m_ViewIds[id] = renderView;
        bgfx::setViewName(renderView->m_Id, name);

        m_RenderViewIds.push_back(id);

        ANNILEEN_LOGF_INFO(LoggingChannel::Renderer, "Added RenderView \"{0}\"", name);

        return true;
    }

    RenderView* RenderView::getRenderView(const size_t id)
    {
        auto renderViewIt = m_ViewIds.find(id);
        if (renderViewIt == m_ViewIds.end())
        {
            ANNILEEN_LOGF_ERROR(LoggingChannel::Renderer, "Could not find RenderView at {0}.", id);
            return nullptr;
        }

        return renderViewIt->second;
    }

    bool RenderView::hasRenderView(const size_t id)
    {
        auto renderViewIt = m_ViewIds.find(id);
        return (renderViewIt != m_ViewIds.end());
    }

    bool RenderView::insertOrMoveRenderViewAfter(const size_t id, const char* name, const size_t afterThisId)
    {
        if (id <= PostProcessing)
        {
            ANNILEEN_LOGF_ERROR(LoggingChannel::Renderer, "RenderView cannot be added, because RenderView id {0} is reserved by the engine.", id);
            return false;
        }

        return insertOrMoveReservedRenderViewAfter(id, name, afterThisId);
    }

    bool RenderView::insertOrMoveRenderViewBefore(const size_t id, const char* name, const size_t beforeThisId)
    {
        if (id <= PostProcessing)
        {
            ANNILEEN_LOGF_ERROR(LoggingChannel::Renderer, "RenderView cannot be added, because RenderView id {0} is reserved by the engine.", id);
            return false;
        }
        
        return insertOrMoveReservedRenderViewBefore(id, name, beforeThisId);
    }
    
    bool RenderView::removeRenderView(const size_t id)
    {
        if (id <= PostProcessing)
        {
            ANNILEEN_LOGF_ERROR(LoggingChannel::Renderer, "RenderView cannot be removed, because RenderView id {0} is reserved by the engine.", id);
            return false;
        }
        
        return removeReservedRenderView(id);
    }

    bool RenderView::insertOrMoveReservedRenderViewAfter(const size_t id, const char* name, const size_t afterThisId)
    {
        auto afterIdIt = std::find(m_RenderViewIds.begin(), m_RenderViewIds.end(), afterThisId);
        if (afterIdIt == m_RenderViewIds.end())
        {
            ANNILEEN_LOGF_ERROR(LoggingChannel::Renderer, "RenderView cannot be inserted. Could not find RenderView at {0}.", afterThisId);
            return false;
        }

        auto idIt = std::find(m_RenderViewIds.begin(), m_RenderViewIds.end(), id);
        if (idIt == m_RenderViewIds.end())
        {
            if (m_ViewIds.size() == maximumNumberOfRenderViews)
            {
                ANNILEEN_LOG_ERROR(LoggingChannel::Renderer, "RenderView cannot be inserted. Maximum number of RenderViews was reached already");
                return false;
            }

            RenderView *renderView = new RenderView(static_cast<bgfx::ViewId>(m_ViewIds.size()), std::string_view(name));

            bgfx::setViewName(renderView->m_Id, name);

            m_ViewIds[id] = renderView;
            m_RenderViewIds.insert(++afterIdIt, id);
        }
        else
        {
            m_RenderViewIds.insert(++afterIdIt, id);
            m_RenderViewIds.erase(idIt);
        }

        redefineViewIds();

        ANNILEEN_LOGF_INFO(LoggingChannel::Renderer, "Added RenderView \"{0}\"", name);

        return true;
    }

    bool RenderView::insertOrMoveReservedRenderViewBefore(const size_t id, const char* name, const size_t beforeThisId)
    {
        auto beforeIdIt = std::find(m_RenderViewIds.begin(), m_RenderViewIds.end(), beforeThisId);
        if (beforeIdIt == m_RenderViewIds.end())
        {
            ANNILEEN_LOGF_ERROR(LoggingChannel::Renderer, "RenderView cannot be inserted. Could not find RenderView at {0}.", beforeThisId);
            return false;
        }

        auto idIt = std::find(m_RenderViewIds.begin(), m_RenderViewIds.end(), id);
        if (idIt == m_RenderViewIds.end())
        {
            if (m_ViewIds.size() == maximumNumberOfRenderViews)
            {
                ANNILEEN_LOG_ERROR(LoggingChannel::Renderer, "RenderView cannot be inserted. Maximum number of RenderViews was reached already");
                return false;
            }

            RenderView *renderView = new RenderView(static_cast<bgfx::ViewId>(m_ViewIds.size()), std::string_view(name));

            bgfx::setViewName(renderView->m_Id, name);

            m_ViewIds[id] = renderView;
            m_RenderViewIds.insert(beforeIdIt, id);
        }
        else
        {
            m_RenderViewIds.insert(beforeIdIt, id);
            m_RenderViewIds.erase(idIt);
        }

        redefineViewIds();

        ANNILEEN_LOGF_INFO(LoggingChannel::Renderer, "Added RenderView \"{0}\"", name);

        return false;
    }

    bool RenderView::removeReservedRenderView(const size_t id)
    {
        // TODO: Add assert, id should always exist in both map and list.

        auto idIt = std::find(m_RenderViewIds.begin(), m_RenderViewIds.end(), id);
        if (idIt == m_RenderViewIds.end())
        {
            ANNILEEN_LOGF_ERROR(LoggingChannel::Renderer, "RenderView {0} cannot be removed, because it does not exist.", id);
            return false;
        }

        m_RenderViewIds.erase(idIt);

        auto viewIdIt = m_ViewIds.find(id);
        if (viewIdIt == m_ViewIds.end())
        {
            ANNILEEN_LOGF_ERROR(LoggingChannel::Renderer, "RenderView {0} cannot be removed, because it does not exist.", id);
            return false;
        }

        RenderView* renderView = viewIdIt->second;

        m_ViewIds.erase(viewIdIt);

        redefineViewIds();

        ANNILEEN_LOGF_INFO(LoggingChannel::Renderer, "Removed RenderView \"{0}\"", renderView->m_Name);

        if (renderView != nullptr)
        {
            delete renderView;
        }

        return true;
    }
}

