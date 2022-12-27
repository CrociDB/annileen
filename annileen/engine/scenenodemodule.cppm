
//#include "engine/core/logger.h"

import scenenodemodule;

import transform;
import scenenode;

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
