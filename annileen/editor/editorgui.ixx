module;

#include <imgui-utils/imgui.h>
#include <glm.hpp>
#include <gtx/matrix_decompose.hpp>
#include <list>
#include <vector>
#include <memory>
#include <dear-imgui/imgui.h>
#include <imgui-utils/imgui_stdlib.h>
#include <engine/core/logger.h>

#include <GLFW/glfw3.h>
// GLFW will include xserver headers in Linux and it defines macro `None`. So including
// this math header before fixes the redefinition of this macro.
#include <bx/math.h>
#if BX_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#elif BX_PLATFORM_OSX
#define GLFW_EXPOSE_NATIVE_COCOA
#endif
#include <GLFW/glfw3native.h>
#include "engine/forward_decl.h"

export module editorgui;

import scene;
import scenenode;

export namespace annileen
{
	
	class EditorGui final
	{
		enum Mode
		{
			Editor,
			Game
		};

		enum class ViewHandleOperation
		{
			Move,
			Rotate,
			Scale
		};

		enum class ViewHandleMode
		{
			Local,
			World
		};

		struct InputConfig
		{
			float mouseSpeed;
			float movementSpeed;
			float sensitivity;
			float yaw;
			float pitch;
			int debugScreenActive;
		};
		InputConfig m_InputConfig;

		std::vector<const char*> m_ConsoleLoggingChannelsList;
		std::vector<const char*> m_ConsoleLoggingLevelsList;

		EditorGui();
		~EditorGui();

		ViewHandleMode m_HandleMode;
		ViewHandleOperation m_HandleOperation;

		bool m_ShowToolsWindow;
		bool m_ShowSceneHierarchyWindow;
		bool m_ShowSceneNodePropertiesWindow;
		bool m_ShowConsoleWindow;
		bool m_ShowSettingsWindow;

		bool m_RenderSkybox;

		bool m_AssetHotreload;

		bool m_HasWindowFocused;

		Mode m_Mode;

		// These will probably become a list when we start allowing multiple selection.
		SceneNode* m_SelectedSceneNode;
		SceneNode* m_SceneNodeToBeRemoved;

		void initialize();
		void processInput(Camera* camera, float deltaTime);
		void render(Scene* scene, Camera* camera, float deltaTime);
		void drawMainWindowToolbar();
		void drawToolsWindow();
		void drawSceneHierarchyWindow(const std::vector<SceneNodePtr> sceneNodeList);
		void drawSelectedNodePropertiesWindow(Camera* camera);
		void drawConsoleWindow();
		void drawSettingsWindow();
		void _drawTree(SceneNodePtr const sceneNode);

		// Modules
		void drawModelModuleProperties(Model* model);
		void drawLightModuleProperties(Light* light);
		void drawCameraModuleProperties(Camera* camera);
		void drawTextModuleProperties(Text* text);
		void drawModelMaterialProperties(std::shared_ptr<Material> material);

		void drawSceneNodeContextMenu(SceneNodePtr const sceneNode);

		SceneNodePtr drawSceneNode(SceneNodePtr const sceneNode, std::string nodeName);

		template <class T>
		void drawNewSceneNodeContextMenu(SceneNodePtr const sceneNode, std::string nodeName);
		friend class ApplicationEditor;
	};

	template <class T>
	void EditorGui::drawNewSceneNodeContextMenu(SceneNodePtr const sceneNode, std::string nodeName)
	{
		SceneNodePtr newSceneNode = drawSceneNode(sceneNode, nodeName);

		if (newSceneNode != nullptr)
		{
			SceneNodeModule* newModule = newSceneNode->addModule<T>();
		}
	}
}
