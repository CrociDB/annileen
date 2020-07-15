#include "scenenode.h"
#include "scene.h"

namespace annileen
{
	void SceneNode::setParentScene(Scene* scene)
	{
		m_ParentScene = scene;
	}

	void SceneNode::setParent(SceneNode* node)
	{
		if (node != nullptr)
			m_Parent = node;
		else
			m_Parent = m_ParentScene->getRoot();

		m_Parent->m_Children.push_back(node);
	}

	SceneNode* SceneNode::getParent()
	{
		return m_Parent;
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

	SceneNode::SceneNode() : m_Parent(nullptr)
	{
		m_Model.reset();
	}

	SceneNode::~SceneNode()
	{
		for (auto children : m_Children)
		{
			delete children;
		}
	}
}
