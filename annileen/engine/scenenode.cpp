#include <algorithm>
#include <engine/scenenode.h>
#include <engine/scene.h>
#include <engine/serviceprovider.h>

namespace annileen
{
	size_t SceneNode::m_IdCount = 0;

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
		if (node != nullptr)
			m_Parent = node;
		else
			m_Parent = m_ParentScene->getRoot();

		m_Parent->m_Children.push_back(node);
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

	void SceneNode::addChild(SceneNodePtr node)
	{
		if (node == nullptr || hasChild(node))
		{
			return;
		}

		node->m_Parent = this;

		m_Children.push_back(node);
	}

	void SceneNode::addChildBefore(SceneNodePtr child, SceneNodePtr nodeAfter)
	{
		if (nodeAfter == nullptr)
		{
			addChild(child);
		}
		else
		{
			auto nodeAfterIt = findChild(nodeAfter);

			if (nodeAfterIt == m_Children.end())
			{
				addChild(child);
			}
			else
			{
				child->m_Parent = this;
				m_Children.insert(nodeAfterIt, child);
			}
		}
	}

	void SceneNode::addChildAfter(SceneNodePtr child, SceneNodePtr nodeBefore)
	{
		if (nodeBefore == nullptr)
		{
			addChild(child);
		}
		else
		{
			auto nodeBeforeIt = findChild(nodeBefore);

			if (nodeBeforeIt == m_Children.end())
			{
				addChild(child);
			}
			else
			{
				child->m_Parent = this;
				m_Children.insert(std::next(nodeBeforeIt), child);
			}
		}
	}

	void SceneNode::removeChild(SceneNodePtr node)
	{
		if (node == nullptr)
		{
			return;
		}

		std::vector<SceneNodePtr>::iterator nodeIt = findChild(node);

		if (nodeIt != m_Children.end())
		{
			for (auto nodeChild : node->getChildren())
			{
				removeChild(nodeChild);
			}
			m_Children.erase(nodeIt);

			delete node;
			node = nullptr;
		}
	}

	std::vector<SceneNodePtr>::iterator SceneNode::findChild(SceneNodePtr node)
	{
		return std::find(m_Children.begin(), m_Children.end(), node);
	}

	bool SceneNode::hasChild(SceneNodePtr node)
	{
		return findChild(node) != m_Children.end();
	}

	SceneNode::SceneNode() : m_Parent(nullptr), m_Active(true), m_ParentScene(nullptr)
	{
		m_Id = m_IdCount++;
	}

	SceneNode::SceneNode(Scene* parentScene, std::string name) : m_Parent(nullptr), m_Active(true), m_ParentScene(parentScene),
		name(name)
	{
		m_Id = m_IdCount++;
		if (m_ParentScene != nullptr)
		{
			m_ParentScene->addNodeToList(this);
		}
	}


	SceneNode::~SceneNode()
	{
		if (m_ParentScene != nullptr)
		{
			m_ParentScene->removeNodeFromList(this);
		}

		for (auto children : m_Children)
		{
			if (children != nullptr)
			{
				delete children;
			}
		}

		m_Children.clear();
	
		for (auto moduleIt : m_Modules)
		{
			SceneNodeModulePtr sceneNodeModule = moduleIt.second;

			if (sceneNodeModule != nullptr)
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

				delete sceneNodeModule;
				sceneNodeModule = nullptr;
			}
		}

		m_Modules.clear();

		m_Parent = nullptr;
		m_ParentScene = nullptr;
	}
}
