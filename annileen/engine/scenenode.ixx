module;

#include <algorithm>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include <memory>
#include <engine/forward_decl.h>

export module scenenode;

import transform;

export namespace annileen
{
	enum SceneNodeFlags
	{
		SceneNodeFlags_Hide = 1
	};

	class SceneNode final
	{

	private:
		Transform m_Transform;
		std::vector<SceneNodePtr> m_Children;
		SceneNodePtr m_Parent;

		bool m_Active;
		// Flag used internally to identify system scene nodes.
		bool m_Internal;
		size_t m_Id;

		std::unordered_map<std::type_index, SceneNodeModule*> m_Modules;

		// TODO: this is temporary id, not safe, not ideal. Replace someday.
		static size_t m_IdCount;

		void deParent();

		SceneNode(const std::string& name);

		friend class Scene;
		friend class SceneManager;
		friend class Application;
		friend class ApplicationEditor;

	public:
		int flags = 0;
		std::string name = "SceneNode";

		// TODO: change this to use string hash.
		std::string tag = "";
		//int hashedTag = 0;

		void setParent(SceneNodePtr node);
		SceneNodePtr getParent();
		std::vector<SceneNodePtr> getChildren();

		void setActive(bool active) { m_Active = active; }
		bool getActive();

		size_t getId() { return m_Id; };

		Transform& getTransform();

		void setSiblingIndex(size_t index);
		void setSiblingPosition(std::vector<SceneNodePtr>::iterator position);

		std::vector<SceneNodePtr>::iterator getSiblingIterator();
		size_t getSiblingIndex();

		std::vector<SceneNodePtr>::iterator findChild(SceneNodePtr node);
		bool hasChild(SceneNodePtr node);

		~SceneNode() = default;
	};
}

namespace annileen
{
	size_t SceneNode::m_IdCount = 0;

	SceneNode::SceneNode(const std::string& name) : m_Parent(nullptr), m_Active(true), name(name)
		, m_Internal(false)
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

	void SceneNode::setParent(SceneNodePtr node)
	{
		if (m_Parent == node) return;
		if (node == nullptr) return;

		deParent();

		m_Parent = node;

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

	bool SceneNode::getActive()
	{
		SceneNode* currentNode = this;
		while (currentNode != nullptr)
		{
			if (currentNode->m_Active == false)
			{
				return false;
			}
			currentNode = currentNode->m_Parent;
		}
		return true;
	}

	void SceneNode::setSiblingIndex(size_t index)
	{
		index = std::min(index, m_Parent->m_Children.size() - 1);
		setSiblingPosition(m_Parent->m_Children.begin() + index);
	}

	void SceneNode::setSiblingPosition(std::vector<SceneNodePtr>::iterator position)
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
}
