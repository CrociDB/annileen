module;

#include <algorithm>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include <memory>
#include "engine/forward_decl.h"

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
		Scene* m_ParentScene;

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
		friend class Application;
		friend class ApplicationEditor;

		void addCameraModule(Camera* camera);
		void addLightModule(Light* light);
		void removeCameraModule(Camera* camera);
		void removeLightModule(Light* light);

	public:
		int flags = 0;
		std::string name = "SceneNode";

		// TODO: change this to use string hash.
		std::string tag = "";
		//int hashedTag = 0;

		void setParentScene(Scene* scene);
		void setParent(SceneNodePtr node);
		SceneNodePtr getParent();
		Scene* getParentScene() { return m_ParentScene; }
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

		template <class T> T* getModule() const;
		template <class T> T* addModule();
		template <class T> bool removeModule();

		~SceneNode();
	};

	// Template implementations

	template <class T>
	T* SceneNode::getModule() const
	{
		auto moduleIt = m_Modules.find(typeid(T));

		if (moduleIt != m_Modules.end())
		{
			return static_cast<T*>(moduleIt->second);
		}

		return nullptr;
	}

	template <class T>
	T* SceneNode::addModule()
	{
		if (!std::is_base_of<SceneNodeModule, T>::value)
		{
			//ANNILEEN_LOGF_ERROR(LoggingChannel::General, "\"{0}\" cannot be added because it is not a module.", typeid(T).name());
			return nullptr;
		}

		auto moduleIt = m_Modules.find(typeid(T));

		if (moduleIt != m_Modules.end())
		{
			//ANNILEEN_LOG_ERROR(LoggingChannel::General, "This SceneNode has a Module of this type already. Remove the existing module before adding a new one of the same type.");
			return nullptr;
		}

		T* module = new T();

		m_Modules[typeid(T)] = static_cast<SceneNodeModulePtr>(module);

		module->m_SceneNode = this;

		if (!m_Internal)
		{
			if constexpr (std::is_same<T, Camera>::value)
			{
				addCameraModule(module);
			}
			else if constexpr (std::is_same<T, Light>::value)
			{
				addLightModule(module);
			}
		}

		return module;
	}

	template <class T>
	bool SceneNode::removeModule()
	{
		auto moduleIt = m_Modules.find(typeid(T));

		if (moduleIt == m_Modules.end())
		{
			//ANNILEEN_LOG_ERROR(LoggingChannel::General, "This SceneNode does not have a Module of this type.");
			return false;
		}

		T* module = static_cast<T*>(moduleIt->second);

		if (module != nullptr)
		{
			module->m_SceneNode = nullptr;

			if (!m_Internal)
			{
				if constexpr (std::is_same<T, Camera>::value)
				{
					removeCameraModule(module);
				}
				else if constexpr (std::is_same<T, Light>::value)
				{
					removeLightModule(module);
				}
			}

			delete module;
		}

		m_Modules.erase(moduleIt);

		return true;
	}
}
