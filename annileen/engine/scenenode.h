#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include <memory>

#include <engine/transform.h>
#include <engine/model.h>


namespace annileen
{
	class Scene;
	class SceneNode;
	class Model;
	class SceneNodeModule;

	// TODO: move this kind of defs to separate header
	typedef SceneNode* SceneNodePtr;
	
	class SceneNode final
	{

	private:
		Transform m_Transform;
		std::vector<SceneNodePtr> m_Children;
		SceneNodePtr m_Parent;
		Scene* m_ParentScene;

		bool m_Active;

		std::unordered_map<std::type_index, SceneNodeModule*> m_Modules;

	public:
		std::string name = "SceneNode";
		
		void setParentScene(Scene* scene);
		void setParent(SceneNodePtr node);
		SceneNodePtr getParent();
		std::vector<SceneNodePtr> getChildren();

		void setAcive(bool active) { m_Active = active; }
		bool getAcive() { return m_Active; }

		Transform& getTransform();

		void addChild(SceneNodePtr node);
		void removeChild(SceneNodePtr node);
		std::vector<SceneNodePtr>::iterator findChild(SceneNodePtr node);
		bool hasChild(SceneNodePtr node);
		void _destroyNode(SceneNodePtr node);

		template <class T> T* getModule() const;
		template <class T> bool addModule(T* module);
		template <class T> bool removeModule();

		SceneNode();
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
	bool SceneNode::addModule(T* module)
	{
		SceneNodeModulePtr sceneNodeModule = static_cast<SceneNodeModulePtr>(module);

		if (sceneNodeModule == nullptr)
		{
			//ANNILEEN_LOGF_ERROR(LoggingChannel::General, "\"{0}\" cannot be added because it is not a module.", typeid(T).name());
			return false;
		}

		auto moduleIt = m_Modules.find(typeid(T));

		if (moduleIt != m_Modules.end())
		{
			//ANNILEEN_LOG_ERROR(LoggingChannel::General, "This SceneNode has a Module of this type already. Remove the existing module before adding a new one of the same type.");
			return false;
		}

		m_Modules[typeid(T)] = module;

		module->m_SceneNode = this;

		// "if constexpr" is a C++17 thing.
		if constexpr (std::is_same<T, Camera>::value)
		{
			m_ParentScene->m_Cameras.push_back(module);
		}
		else if constexpr (std::is_same<T, Light>::value)
		{
			m_ParentScene->m_Lights.push_back(module);
		}

		return true;
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

			if constexpr (std::is_same<T, Camera>::value)
			{
				m_ParentScene->m_Cameras.remove(module);
			}
			else if constexpr (std::is_same<T, Light>::value)
			{
				m_ParentScene->m_Lights.remove(module);
			}

			delete module;
		}

		m_Modules.erase(moduleIt);

		return true;
	}

}
