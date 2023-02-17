module;

export module scenenodemodule;

import transform;
import scenenode;

export namespace annileen
{
	class SceneNodeModule
	{
	private:
		friend class SceneManager;
		friend class SceneNode;

	public:
		SceneNodeModule() = default;
		virtual ~SceneNodeModule() = default;

		Transform& getTransform();
		SceneNode* getSceneNode();
	
	private:
		// Reference to scene node where module is attached.
		SceneNode* m_SceneNode;
	};
}

namespace annileen
{
	Transform& SceneNodeModule::getTransform()
	{
		// TODO: uncomment after serviceprovider is converted to module;
		/*if (m_SceneNode == nullptr)
		{
			ANNILEEN_LOG_ERROR(LoggingChannel::General, "Cannot get correct SceneNodeModule transform because it is not attached to a SceneNode.");
			exit(-1);
		}*/

		return m_SceneNode->getTransform();
	}

	SceneNode* SceneNodeModule::getSceneNode()
	{
		return m_SceneNode;
	}
}
