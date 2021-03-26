#include <engine/scenenodemodule.h>
#include <engine/serviceprovider.h>
#include <engine/transform.h>
#include <engine/scenenode.h>

namespace annileen
{
	Transform& SceneNodeModule::getTransform()
	{
		if (m_SceneNode == nullptr)
		{
			ANNILEEN_LOG_ERROR(LoggingChannel::General, "Cannot get correct SceneNodeModule transform because it is not attached to a SceneNode.");
			exit(-1);
		}

		return m_SceneNode->getTransform();
	}
	SceneNode* SceneNodeModule::getSceneNode()
	{
		return m_SceneNode;
	}
}