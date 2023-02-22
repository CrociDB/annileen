module;

#include <algorithm>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include <memory>

export module scenenode;

import transform;

export namespace annileen
{
	enum SceneNodeFlags
	{
		SceneNodeFlags_Hide = 1
	};

	class SceneNodeModule;

	class SceneNode final : public std::enable_shared_from_this<SceneNode>
	{

	private:
		Transform m_Transform;
		std::vector<std::shared_ptr<SceneNode>> m_Children;
		std::shared_ptr<SceneNode> m_Parent;

		bool m_Active;
		// Flag used internally to identify system scene nodes.
		bool m_Internal;
		size_t m_Id;

		std::unordered_map<std::type_index, std::shared_ptr<SceneNodeModule>> m_Modules;

		// TODO: this is temporary id, not safe, not ideal. Replace someday.
		static size_t m_IdCount;

		void deParent();

		friend class Scene;
		friend class SceneManager;
		friend class Application;
		friend class ApplicationEditor;

	public:
		SceneNode(const std::string& name);
		int flags = 0;
		std::string name = "SceneNode";

		// TODO: change this to use string hash.
		std::string tag = "";
		//int hashedTag = 0;

		void setParent(std::shared_ptr<SceneNode> node);
		std::shared_ptr<SceneNode> getParent();
		std::vector<std::shared_ptr<SceneNode>> getChildren();

		void setActive(bool active) { m_Active = active; }
		bool getActive();

		size_t getId() { return m_Id; };

		Transform& getTransform();

		void setSiblingIndex(size_t index);
		void setSiblingPosition(std::vector<std::shared_ptr<SceneNode>>::iterator position);

		std::vector<std::shared_ptr<SceneNode>>::iterator getSiblingIterator();
		size_t getSiblingIndex();

		std::vector<std::shared_ptr<SceneNode>>::iterator findChild(std::shared_ptr<SceneNode> node);
		bool hasChild(std::shared_ptr<SceneNode> node);

		~SceneNode();
	};

	class SceneNodeModule
	{
	private:
		friend class SceneManager;
		friend class SceneNode;

	public:
		SceneNodeModule() = default;
		virtual ~SceneNodeModule()
		{
			// TODO: remove
			std::cout << "SceneNodeModule destroyed" << std::endl;
		};

		Transform& getTransform() const noexcept;
		std::shared_ptr<SceneNode> getSceneNode() const noexcept;

	private:
		// Reference to scene node where module is attached.
		std::shared_ptr<SceneNode> m_SceneNode;
	};
}

namespace annileen
{
	size_t SceneNode::m_IdCount{ 0 };

	SceneNode::SceneNode(const std::string& name) : m_Parent(nullptr), m_Active(true), name(name)
		, m_Internal(false)
	{
		m_Id = m_IdCount++;
	}

	SceneNode::~SceneNode()
	{
		// TODO: remove
		std::cout << "SceneNode " << name << " destroyed" << std::endl;
	}

	void SceneNode::deParent()
	{
		if (m_Parent != nullptr)
			m_Parent->m_Children.erase(
				std::remove(m_Parent->m_Children.begin(), m_Parent->m_Children.end(), shared_from_this()),
				m_Parent->m_Children.end());
	}

	void SceneNode::setParent(std::shared_ptr<SceneNode> node)
	{
		if (m_Parent == node) return;
		if (node == nullptr) return;

		deParent();

		m_Parent = node;

		m_Parent->m_Children.push_back(shared_from_this());
	}

	std::shared_ptr<SceneNode> SceneNode::getParent()
	{
		return m_Parent;
	}

	std::vector<std::shared_ptr<SceneNode>> SceneNode::getChildren()
	{
		return m_Children;
	}

	Transform& SceneNode::getTransform()
	{
		return m_Transform;
	}

	bool SceneNode::getActive()
	{
		std::shared_ptr<SceneNode> currentNode = shared_from_this();
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

	void SceneNode::setSiblingPosition(std::vector<std::shared_ptr<SceneNode>>::iterator position)
	{
		std::rotate(position, getSiblingIterator(), m_Parent->m_Children.end());
	}

	std::vector<std::shared_ptr<SceneNode>>::iterator SceneNode::getSiblingIterator()
	{
		auto it = std::find(m_Parent->m_Children.begin(), m_Parent->m_Children.end(), shared_from_this());
		return it;
	}

	size_t SceneNode::getSiblingIndex()
	{
		return std::distance(
			m_Parent->m_Children.begin(),
			std::find(m_Parent->m_Children.begin(), m_Parent->m_Children.end(), shared_from_this()));
	}

	std::vector<std::shared_ptr<SceneNode>>::iterator SceneNode::findChild(std::shared_ptr<SceneNode> node)
	{
		return std::find(m_Children.begin(), m_Children.end(), node);
	}

	bool SceneNode::hasChild(std::shared_ptr<SceneNode> node)
	{
		return findChild(node) != m_Children.end();
	}

	Transform& SceneNodeModule::getTransform() const noexcept
	{
		if (m_SceneNode == nullptr)
		{
			// TODO: uncomment after serviceprovider is converted to module;
			//ANNILEEN_LOG_ERROR(LoggingChannel::General, "Cannot get correct SceneNodeModule transform because it is not attached to a SceneNode.");
			exit(-1);
		}

		return m_SceneNode->getTransform();
	}

	std::shared_ptr<SceneNode> SceneNodeModule::getSceneNode() const noexcept
	{
		return m_SceneNode;
	}
}
