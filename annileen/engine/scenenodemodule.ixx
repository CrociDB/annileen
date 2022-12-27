module;

#include "engine/forward_decl.h"

export module scenenodemodule;

export namespace annileen
{
	class SceneNodeModule
	{
	private:

		// Reference to scene node where module is attached.
		SceneNode* m_SceneNode;

		friend class SceneNode;

	public:

		SceneNodeModule() {}
		virtual ~SceneNodeModule() {}

		Transform& getTransform();
		SceneNode* getSceneNode();
	};
}