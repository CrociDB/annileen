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

		void init(bool isStatic, bool sdf = false);
		void applyProperties();
		
		void render(bgfx::ViewId viewId);

		void setStatic(bool isStatic);
		void setSdf(bool isSdf);

		bool isSdf() { return m_Sdf; }
		bool isStatic() { return m_IsStatic; }

		void setFont(TrueTypeHandle font);
		void setPixelSize(uint32_t pixelSize);
		uint32_t getPixelSize() { return m_PixelSize; }

		void setText(const std::string text);
		std::string getText() { return m_Text; }

		void setScreenPosition(float x, float y);
		glm::vec2 getScreenPosition() { return m_ScreenPosition; }

		void setBackgroundColor(glm::vec3 backgroundColor);
		glm::vec3 getBackgroundColor() { return m_BackgroundColor; }

		void setTextColor(glm::vec3 textColor);
		glm::vec3 getTextColor() { return m_TextColor; }

		void setUnderlineColor(glm::vec3 underlineColor);
		glm::vec3 getUnderlineColor() { return m_UnderlineColor; }

		void setOverlineColor(glm::vec3 overlineColor);
		glm::vec3 getOverlineColor() { return m_OverlineColor; }

		void setStrikeThroughColor(glm::vec3 strikeThroughColor);
		glm::vec3 getStrikeThroughColor() { return m_StrikeThroughColor; }

		void setStyle(TextStyle textStyle);
		TextStyle getStyle() { return m_TextStyle; }

		Text();
		~Text();
	};

	typedef Text* TextPtr;
}