#include <algorithm>
#include <engine/scenenode.h>
#include <engine/scene.h>
#include <engine/serviceprovider.h>

namespace annileen
{
	void SceneNode::setParentScene(Scene* scene)
	{
		m_ParentScene = scene;
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

	void SceneNode::removeChild(SceneNodePtr node)
	{
		if (node == nullptr)
		{
			return;
		}

		std::vector<SceneNodePtr>::iterator nodeIt = findChild(node);

		if (nodeIt != m_Children.end())
		{
			_destroyNode(node);
			m_Children.erase(nodeIt);
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

	void SceneNode::_destroyNode(SceneNodePtr node)
	{
		// TODO:
	}

	SceneNode::SceneNode() : m_Parent(nullptr), m_Active(true), m_ParentScene(nullptr)
	{
	}

	SceneNode::~SceneNode()
	{
		/*for (auto children : m_Children)
		{
			delete children;
		}*/
	
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
	}

	
}
