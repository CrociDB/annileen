#pragma once

#include <iostream>
#include <vector>

#include "transform.h"
#include "model.h"

namespace annileen
{
	class Scene;

	class SceneNode
	{
	private:
		Transform m_Transform;
		std::vector<SceneNode*> m_Children;
		SceneNode* m_Parent;
		Scene* m_ParentScene;
		std::string m_Name = "SceneNode";

		std::shared_ptr<Model> m_Model;

		bool m_Active;

	public:
		void setParentScene(Scene* scene);
		void setParent(SceneNode* node);
		SceneNode* getParent();
		std::vector<SceneNode*> getChildren();

		void setAcive(bool active) { m_Active = active; }
		bool getAcive() { return m_Active; }

		Transform& getTransform();

		bool hasModel();
		void setModel(std::shared_ptr<Model> model);
		std::shared_ptr<Model> getModel();

		std::string getName() const;

		SceneNode();
		~SceneNode();
	};
}
