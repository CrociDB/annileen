module;

#include <algorithm>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include <memory>

namespace annileen
{
	class Renderer;
	class Gui;
	class Input;
	class Uniform;
	class Camera;
	class Scene;
	class SceneNodeModule;
	class Light;
	class Skybox;
	class Model;
	class Text;
	class Material;

	using SceneNodeModulePtr = SceneNodeModule*;
	using ModelPtr = Model*;
	using CameraPtr = Camera*;
	using TextPtr = Text*;
}

export module scenenode;

import transform;

export namespace annileen
{
	enum SceneNodeFlags
	{
		SceneNodeFlags_Hide = 1
	};

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

		std::unordered_map<std::type_index, SceneNodeModule*> m_Modules;

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
}
