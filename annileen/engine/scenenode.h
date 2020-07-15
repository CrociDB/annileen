#pragma once

#include <iostream>
#include <vector>

#include "transform.h"
#include "model.h"

namespace annileen
{
	class SceneNode
	{
	private:
		Transform m_Transform;
		std::vector<SceneNode*> m_Children;
		SceneNode* m_Parent;

		std::shared_ptr<Model> m_Model;

	public:

		void setParent(SceneNode* node);
		SceneNode* getParent();

		Transform& getTransform();

		void setModel(std::shared_ptr<Model> model);
		std::shared_ptr<Model> getModel();

		SceneNode();
		~SceneNode();
	};
}
