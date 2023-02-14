#pragma once

namespace annileen
{
	class Renderer;
	class Gui;
	class Input;
	class Uniform;
	class Camera;
	class Scene;
	class SceneNode;
	class SceneNodeModule;
	class Light;
	class Skybox;
	class Transform;
	class Model;
	class Text;
	class Material;

	using SceneNodePtr = SceneNode*;
	using SceneNodeModulePtr = SceneNodeModule*;
	using ModelPtr = Model*;
	using CameraPtr = Camera*;
	using TextPtr = Text*;
}