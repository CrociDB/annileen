#pragma once

namespace annileen
{
	class SceneNode;
	class Transform;

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
	};

	typedef SceneNodeModule* SceneNodeModulePtr;
}