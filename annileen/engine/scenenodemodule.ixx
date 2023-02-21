module;

#include <memory>
#include <iostream>

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
		virtual ~SceneNodeModule()
		{
			// TODO: remove
			std::cout << "SceneNodeModule destroyed" << std::endl;
		};

		Transform& getTransform() const noexcept;
		std::shared_ptr<SceneNode> getSceneNode() const noexcept;
	
	private:
		// Reference to scene node where module is attached.
		std::shared_ptr<SceneNode> m_SceneNode;
	};
}

namespace annileen
{
	Transform& SceneNodeModule::getTransform() const noexcept
	{
		if (m_SceneNode == nullptr)
		{
			// TODO: uncomment after serviceprovider is converted to module;
			//ANNILEEN_LOG_ERROR(LoggingChannel::General, "Cannot get correct SceneNodeModule transform because it is not attached to a SceneNode.");
			exit(-1);
		}

		return m_SceneNode->getTransform();
	}

	std::shared_ptr<SceneNode> SceneNodeModule::getSceneNode() const noexcept
	{
		return m_SceneNode;
	}
}
