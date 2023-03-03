module;

#include <algorithm>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include <memory>

export module anni.scenenode;

import anni.transform;

export namespace annileen
{
	enum SceneNodeFlags
	{
		SceneNodeFlags_Hide = 1
	};

	class SceneNodeModule;

	class SceneNode final : public std::enable_shared_from_this<SceneNode>
	{
		friend class Scene;
		friend class SceneManager;
		friend class Application;
		friend class ApplicationEditor;
	
		// TODO: this is temporary id, not safe, not ideal. Replace someday.
		static size_t m_IdCount;

	public:
		SceneNode(const std::string& name);
		~SceneNode();

		void setParent(std::shared_ptr<SceneNode> node);
		std::shared_ptr<SceneNode>& getParent() noexcept;
		std::vector<std::shared_ptr<SceneNode>>& getChildren() noexcept;

		void setActive(bool active) noexcept;
		bool getActive() noexcept;
		size_t getId() const noexcept;
		Transform& getTransform() noexcept;

		void setSiblingIndex(size_t index);
		void setSiblingPosition(const std::vector<std::shared_ptr<SceneNode>>::iterator& position);
		std::vector<std::shared_ptr<SceneNode>>::iterator getSiblingIterator();
		size_t getSiblingIndex();

		std::vector<std::shared_ptr<SceneNode>>::iterator findChild(const std::shared_ptr<SceneNode>& node);
		bool hasChild(const std::shared_ptr<SceneNode>& node);

	private:
		void deParent();

	public:		
		int flags{ 0 };
		std::string name{ "SceneNode" };

		// TODO: change this to use string hash.
		std::string tag{ "" };
		//int hashedTag = 0;

	private:
		Transform m_Transform{};
		std::vector<std::shared_ptr<SceneNode>> m_Children{};
		std::shared_ptr<SceneNode> m_Parent{};

		bool m_Active{ true };
		// Flag used internally to identify system scene nodes.
		bool m_Internal{ false };
		size_t m_Id{ 0 };

		std::unordered_map<std::type_index, std::shared_ptr<SceneNodeModule>> m_Modules{};
	};

	class SceneNodeModule
	{
		friend class SceneManager;
		friend class SceneNode;

	public:
		SceneNodeModule() = default;
		virtual ~SceneNodeModule();

		Transform& getTransform() noexcept;
		std::shared_ptr<SceneNode> getSceneNode() noexcept;

	private:
		// Reference to scene node where module is attached.
		std::weak_ptr<SceneNode> m_SceneNode{};
	};
}

module :private;

namespace annileen
{
	size_t SceneNode::m_IdCount{ 0 };

	SceneNode::SceneNode(const std::string& name) : name(name)
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

	std::shared_ptr<SceneNode>& SceneNode::getParent() noexcept
	{
		return m_Parent;
	}

	std::vector<std::shared_ptr<SceneNode>>& SceneNode::getChildren() noexcept
	{
		return m_Children;
	}

	Transform& SceneNode::getTransform() noexcept
	{
		return m_Transform;
	}

	bool SceneNode::getActive() noexcept
	{
		auto currentNode{ shared_from_this() };
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

	void SceneNode::setActive(bool active) noexcept 
	{ 
		m_Active = active; 
	}
	
	size_t SceneNode::getId() const noexcept 
	{ 
		return m_Id; 
	};

	void SceneNode::setSiblingIndex(size_t index)
	{
		index = std::min(index, m_Parent->m_Children.size() - 1);
		setSiblingPosition(m_Parent->m_Children.begin() + index);
	}

	void SceneNode::setSiblingPosition(const std::vector<std::shared_ptr<SceneNode>>::iterator& position)
	{
		std::rotate(position, getSiblingIterator(), m_Parent->m_Children.end());
	}

	std::vector<std::shared_ptr<SceneNode>>::iterator SceneNode::getSiblingIterator()
	{
		return std::find(m_Parent->m_Children.begin(), m_Parent->m_Children.end(), shared_from_this());		
	}

	size_t SceneNode::getSiblingIndex()
	{
		return std::distance(m_Parent->m_Children.begin(), getSiblingIterator());
	}

	std::vector<std::shared_ptr<SceneNode>>::iterator SceneNode::findChild(const std::shared_ptr<SceneNode>& node)
	{
		return std::find(m_Children.begin(), m_Children.end(), node);
	}

	bool SceneNode::hasChild(const std::shared_ptr<SceneNode>& node)
	{
		return findChild(node) != m_Children.end();
	}

	Transform& SceneNodeModule::getTransform() noexcept
	{
		auto sceneNode{ m_SceneNode.lock() };

		if (sceneNode == nullptr)
		{
			exit(-1);
		}

		return sceneNode->getTransform();
	}

	std::shared_ptr<SceneNode> SceneNodeModule::getSceneNode() noexcept
	{
		return m_SceneNode.lock();
	}

	SceneNodeModule::~SceneNodeModule()
	{
		// TODO: remove
		std::cout << "SceneNodeModule destroyed" << std::endl;
	};

}
