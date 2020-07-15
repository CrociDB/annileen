#include "scenenode.h"

namespace annileen
{
	void SceneNode::setParent(SceneNode* node)
	{
		m_Parent = node;
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
		
	}

	SceneNode::~SceneNode()
	{
		
	}
}
