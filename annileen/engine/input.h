#pragma once

#include <map>
#include <glm.hpp>

namespace annileen
{
	struct KeyData
	{
		bool m_KeyValue;
		bool m_LastKeyValue;
	};

	class Input
	{
	private:
		std::map<int, KeyData> m_Keymap;
		glm::vec2 m_MousePosition;
		glm::vec2 m_MouseScroll;
		glm::vec2 m_MouseDelta;

		bool m_MouseButtons[3];

		void initKeymap();

		bool m_Enabled;

#ifdef _DEBUG
		friend class EditorGui;
#endif
		friend class Application;

		bool _getKey(int keycode) const;
		bool _getKeyDown(int keycode) const;
		bool _getKeyUp(int keycode) const;
		bool _getMouseButtonDown(int button) const;
		glm::vec2 _getMousePosition() const;
		glm::vec2 _getMouseDelta() const;
		glm::vec2 _getMouseScroll() const;

	public:
		bool getKey(int keycode) const;
		bool getKeyDown(int keycode) const;
		bool getKeyUp(int keycode) const;
		bool getMouseButtonDown(int button) const;
		glm::vec2 getMousePosition() const;
		glm::vec2 getMouseDelta() const;
		glm::vec2 getMouseScroll() const;

		void flushEvents();

		void _setKeyDown(int keycode, bool pressed);

		void _setMousePosition(float x, float y);
		void _setMouseScroll(float x, float y);
		void _setMouseDelta(int x, int y);
		void _setMouseButton(int button, bool status);

		Input();
		~Input();
	};
}
