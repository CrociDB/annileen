#include <engine/scenenode.h>
#include <engine/scene.h>
#include <algorithm>

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

	bool SceneNode::hasModel()
	{
		return m_Model.use_count() >= 0;
	}

	void SceneNode::setModel(std::shared_ptr<Model> model)
	{
		m_Model = model;
	}

	std::shared_ptr<Model> SceneNode::getModel()
	{
		return m_Model;
	}

	std::string SceneNode::getName() const
	{
		return m_Name;
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

	SceneNode::SceneNode() : m_Parent(nullptr), m_Active(true)
	{
		m_Model.reset();
	}

	SceneNode::~SceneNode()
	{
		/*for (auto children : m_Children)
		{
			delete children;
		}*/
	}
}
