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

		std::shared_ptr<Model> m_Model;

	public:
		void setParentScene(Scene* scene);
		void setParent(SceneNode* node);
		SceneNode* getParent();

		Transform& getTransform();

		bool hasModel();
		void setModel(std::shared_ptr<Model> model);
		std::shared_ptr<Model> getModel();

		SceneNode();
		~SceneNode();
	};
}
