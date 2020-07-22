#pragma once

#include <iostream>
#include <vector>

#include "transform.h"
#include "model.h"

namespace annileen
{
	class Scene;
	class SceneNode;
	typedef SceneNode* SceneNodePtr;
	
	class SceneNode
	{

	private:
		Transform m_Transform;
		std::vector<SceneNodePtr> m_Children;
		SceneNodePtr m_Parent;
		Scene* m_ParentScene;
		std::string m_Name = "SceneNode";

		std::shared_ptr<Model> m_Model;

		bool m_Active;

	public:
		void setParentScene(Scene* scene);
		void setParent(SceneNodePtr node);
		SceneNodePtr getParent();
		std::vector<SceneNodePtr> getChildren();

		void setAcive(bool active) { m_Active = active; }
		bool getAcive() { return m_Active; }

		Transform& getTransform();

		bool hasModel();
		void setModel(std::shared_ptr<Model> model);
		std::shared_ptr<Model> getModel();

		std::string getName() const;

		void addChild(SceneNodePtr node);
		void removeChild(SceneNodePtr node);
		std::vector<SceneNodePtr>::iterator findChild(SceneNodePtr node);
		bool hasChild(SceneNodePtr node);
		void _destroyNode(SceneNodePtr node);

		SceneNode();
		~SceneNode();
	};
}
