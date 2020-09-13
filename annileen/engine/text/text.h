#pragma once

#include <engine/scenenodemodule.h>
#include <engine/text/fontmanager.h>
#include <engine/text/textbuffermanager.h>
#include <glm.hpp>
#include <string>

namespace annileen
{
	class Text final : public SceneNodeModule
	{
	public:

		enum TextStyle
		{
			Normal = STYLE_NORMAL,
			Background = STYLE_BACKGROUND,
			Underline = STYLE_UNDERLINE,
			Overline = STYLE_OVERLINE,
			StrikeThrough = STYLE_STRIKE_THROUGH
		};

	private:
		TrueTypeHandle m_Font;
		uint32_t m_PixelSize = 32;
		FontHandle m_FontHandle;
		TextBufferHandle m_TextBufferHandle;
		glm::vec2 m_ScreenPosition;
		std::string m_Text;
		bool m_Sdf;

		void createFont();

		bool m_IsStatic;
		bool m_HasSubmittedOnce;

		glm::vec3 m_BackgroundColor;
		glm::vec3 m_TextColor;
		glm::vec3 m_UnderlineColor;
		glm::vec3 m_OverlineColor;
		glm::vec3 m_StrikeThroughColor;

		TextStyle m_TextStyle;
	public:

		bool enabled;

		void init(bool isStatic);
		
		void render(bgfx::ViewId viewId);

		void setFont(TrueTypeHandle font, bool sdf = false);
		void setPixelSize(uint32_t pixelSize);
		void setText(const std::string text);
		void setScreenPosition(float x, float y);

		void setBackgroundColor(glm::vec3 backgroundColor);
		void setTextColor(glm::vec3 textColor);
		void setUnderlineColor(glm::vec3 underlineColor);
		void setOverlineColor(glm::vec3 overlineColor);
		void setStrikeThroughColor(glm::vec3 strikeThroughColor);
		void setStyle(TextStyle textStyle);

		Text();
		~Text();
	};

	typedef Text* TextPtr;
}