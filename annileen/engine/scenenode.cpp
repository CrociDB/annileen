#include <algorithm>
#include <engine/scenenode.h>
#include <engine/scene.h>
#include <engine/serviceprovider.h>

namespace annileen
{
	size_t SceneNode::m_IdCount = 0;

	SceneNode::SceneNode(const std::string& name) : m_ParentScene(nullptr), m_Parent(nullptr), m_Active(true), name(name)
	{
		m_Id = m_IdCount++;
	}

	void SceneNode::deParent()
	{
		if (m_Parent != nullptr)
			m_Parent->m_Children.erase(
				std::remove(m_Parent->m_Children.begin(), m_Parent->m_Children.end(), this),
				m_Parent->m_Children.end());
	}

	void SceneNode::setParentScene(Scene* scene)
	{
		if (m_ParentScene != nullptr)
		{
			m_ParentScene->removeNodeFromList(this);
		}

		if (scene != nullptr)
		{
			m_ParentScene = scene;
			m_ParentScene->addNodeToList(this);
		}
	}

	void SceneNode::setParent(SceneNodePtr node)
	{
		if (m_Parent == node) return;

		deParent();

		if (node != nullptr)
			m_Parent = node;
		else
			m_Parent = m_ParentScene->getRoot();

		m_Parent->m_Children.push_back(this);
	}

	SceneNodePtr SceneNode::getParent()
	{
		return m_Parent;
	}

	std::vector<SceneNodePtr> SceneNode::getChildren()
	{
		return m_Children;
	}

	Transform& SceneNode::getTransform()
	{
		return m_Transform;
	}

	void SceneNode::setSiblingIndex(size_t index)
	{
		index = std::min(index, m_Parent->m_Children.size() - 1);
		setSiblingPosition(m_Parent->m_Children.begin() + index);
	}

	void SceneNode::setSiblingPosition(std::vector<SceneNodePtr>::iterator& position)
	{
		std::rotate(position, getSiblingIterator(), m_Parent->m_Children.end());
	}

	std::vector<SceneNodePtr>::iterator SceneNode::getSiblingIterator()
	{
		auto it = std::find(m_Parent->m_Children.begin(), m_Parent->m_Children.end(), this);
		return it;
	}

	size_t SceneNode::getSiblingIndex()
	{
		return std::distance(
			m_Parent->m_Children.begin(),
			std::find(m_Parent->m_Children.begin(), m_Parent->m_Children.end(), this));
	}

	std::vector<SceneNodePtr>::iterator SceneNode::findChild(SceneNodePtr node)
	{
		return std::find(m_Children.begin(), m_Children.end(), node);
	}

	bool SceneNode::hasChild(SceneNodePtr node)
	{
		return findChild(node) != m_Children.end();
	}


	SceneNode::~SceneNode()
	{
		if (m_ParentScene != nullptr)
		{
			m_ParentScene->removeNodeFromList(this);
		}

		for (auto child : m_Children)
		{
			if (child != nullptr)
			{
				m_ParentScene->destroyNode(child);
			}
		}

		m_Children.clear();
	
		for (auto moduleIt : m_Modules)
		{
			SceneNodeModulePtr sceneNodeModule = moduleIt.second;

			if (sceneNodeModule != nullptr)
			{
				if (m_ParentScene != nullptr)
				{
					if (moduleIt.first == typeid(Camera))
					{
						Camera* camera = static_cast<Camera*>(sceneNodeModule);
						m_ParentScene->m_Cameras.remove(camera);
						camera = nullptr;
					}
					else if (moduleIt.first == typeid(Light))
					{
						Light* light = static_cast<Light*>(sceneNodeModule);
						m_ParentScene->m_Lights.remove(light);
						light = nullptr;
					}
				}

				delete sceneNodeModule;
				sceneNodeModule = nullptr;
			}
		}

		deParent();
		setParentScene(nullptr);

		m_Modules.clear();

		m_Parent = nullptr;
		m_ParentScene = nullptr;
	}
}
