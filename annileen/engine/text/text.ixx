module;

#include <engine/text/fontmanager.h>
#include <engine/text/textbuffermanager.h>
#include <engine/core/logger.h>
#include <engine/forward_decl.h>
#include <glm.hpp>
#include <string>

export module text;

import serviceprovider;
import utils;
import scenenode;

export namespace annileen
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
}


namespace annileen
{
	void Text::createFont()
	{
		// If font is not defined or valid, use font default.
		if (!isValid(m_Font))
		{
			m_Font = ServiceProvider::getAssetManager()->getFont(
				ServiceProvider::getSettings()->getData()->defaultFont)->getHandle();
		}

		if (isValid(m_FontHandle))
		{
			Font::getFontManager()->destroyFont(m_FontHandle);
		}

		if (m_Sdf)
		{
			m_FontHandle = Font::getFontManager()->createFontByPixelSize(m_Font, 0, m_PixelSize, FONT_TYPE_DISTANCE);
		}
		else
		{
			m_FontHandle = Font::getFontManager()->createFontByPixelSize(m_Font, 0, m_PixelSize);
		}

		// Preload glyphs and blit them to atlas.
		//fontManager->preloadGlyph(m_FontHandle, L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ. \n");
	}

	void Text::setFont(TrueTypeHandle font)
	{
		m_Font = font;

		createFont();
	}

	void Text::setPixelSize(uint32_t pixelSize)
	{
		m_PixelSize = pixelSize;

		if (isValid(m_Font))
		{
			createFont();
		}
	}

	void Text::setText(const std::string text)
	{
		if (!isValid(m_TextBufferHandle))
		{
			ANNILEEN_LOG_ERROR(LoggingChannel::Renderer, "Text cannot be set because text buffer is not valid.");
			return;
		}

		if (!isValid(m_FontHandle))
		{
			ANNILEEN_LOG_ERROR(LoggingChannel::Renderer, "Text cannot be set because font is not valid.");
			return;
		}

		if (m_IsStatic && m_HasSubmittedOnce)
		{
			ANNILEEN_LOG_WARNING(LoggingChannel::Renderer, "Text cannot be set because text is static and has been submitted once already.");
			return;
		}

		m_Text = text;
		Font::getTextBufferManager()->clearTextBuffer(m_TextBufferHandle);
		Font::getTextBufferManager()->setPenPosition(m_TextBufferHandle, m_ScreenPosition.x, m_ScreenPosition.y);
		Font::getTextBufferManager()->appendText(m_TextBufferHandle, m_FontHandle, text.c_str());
	}

	void Text::setScreenPosition(float x, float y)
	{
		if (!isValid(m_TextBufferHandle))
		{
			ANNILEEN_LOG_ERROR(LoggingChannel::Renderer, "New position cannot be set because text buffer is not valid.");
			return;
		}

		if (m_IsStatic && m_HasSubmittedOnce)
		{
			ANNILEEN_LOG_WARNING(LoggingChannel::Renderer, "New position cannot be set because text is static and has been submitted once already.");
			return;
		}

		m_ScreenPosition = glm::vec2(x, y);
		Font::getTextBufferManager()->setPenPosition(m_TextBufferHandle, x, y);
	}

	void Text::setBackgroundColor(glm::vec3 backgroundColor)
	{
		if (!isValid(m_TextBufferHandle))
		{
			ANNILEEN_LOG_ERROR(LoggingChannel::Renderer, "Color cannot be set because text buffer is not valid.");
			return;
		}

		if (m_IsStatic && m_HasSubmittedOnce)
		{
			ANNILEEN_LOG_WARNING(LoggingChannel::Renderer, "Color cannot be set because text is static and has been submitted once already.");
			return;
		}

		m_BackgroundColor = backgroundColor;
		Font::getTextBufferManager()->setBackgroundColor(m_TextBufferHandle, convert_color_vec3_uint32(backgroundColor));
	}

	void Text::setTextColor(glm::vec3 textColor)
	{
		if (!isValid(m_TextBufferHandle))
		{
			ANNILEEN_LOG_ERROR(LoggingChannel::Renderer, "Color cannot be set because text buffer is not valid.");
			return;
		}

		if (m_IsStatic && m_HasSubmittedOnce)
		{
			ANNILEEN_LOG_WARNING(LoggingChannel::Renderer, "Color cannot be set because text is static and has been submitted once already.");
			return;
		}

		m_TextColor = textColor;
		Font::getTextBufferManager()->setTextColor(m_TextBufferHandle, convert_color_vec3_uint32(textColor));
	}

	void Text::setUnderlineColor(glm::vec3 underlineColor)
	{
		if (!isValid(m_TextBufferHandle))
		{
			ANNILEEN_LOG_ERROR(LoggingChannel::Renderer, "Color cannot be set because text buffer is not valid.");
			return;
		}

		if (m_IsStatic && m_HasSubmittedOnce)
		{
			ANNILEEN_LOG_WARNING(LoggingChannel::Renderer, "Color cannot be set because text is static and has been submitted once already.");
			return;
		}

		m_UnderlineColor = underlineColor;
		Font::getTextBufferManager()->setUnderlineColor(m_TextBufferHandle, convert_color_vec3_uint32(underlineColor));
	}

	void Text::setOverlineColor(glm::vec3 overlineColor)
	{
		if (!isValid(m_TextBufferHandle))
		{
			ANNILEEN_LOG_ERROR(LoggingChannel::Renderer, "Color cannot be set because text buffer is not valid.");
			return;
		}

		if (m_IsStatic && m_HasSubmittedOnce)
		{
			ANNILEEN_LOG_WARNING(LoggingChannel::Renderer, "Color cannot be set because text is static and has been submitted once already.");
			return;
		}

		m_OverlineColor = overlineColor;
		Font::getTextBufferManager()->setOverlineColor(m_TextBufferHandle, convert_color_vec3_uint32(overlineColor));
	}

	void Text::setStrikeThroughColor(glm::vec3 strikeThroughColor)
	{
		if (!isValid(m_TextBufferHandle))
		{
			ANNILEEN_LOG_ERROR(LoggingChannel::Renderer, "Color cannot be set because text buffer is not valid.");
			return;
		}

		if (m_IsStatic && m_HasSubmittedOnce)
		{
			ANNILEEN_LOG_WARNING(LoggingChannel::Renderer, "Color cannot be set because text is static and has been submitted once already.");
			return;
		}

		m_StrikeThroughColor = strikeThroughColor;
		Font::getTextBufferManager()->setStrikeThroughColor(m_TextBufferHandle, convert_color_vec3_uint32(strikeThroughColor));
	}

#if _MSC_VER && !__INTEL_COMPILER
#pragma warning(push)
#pragma warning(disable : 26812) 
#endif
	void Text::setStyle(TextStyle textStyle)
	{
		if (!isValid(m_TextBufferHandle))
		{
			ANNILEEN_LOG_ERROR(LoggingChannel::Renderer, "Style cannot be set because text buffer is not valid.");
			return;
		}

		if (m_IsStatic && m_HasSubmittedOnce)
		{
			ANNILEEN_LOG_WARNING(LoggingChannel::Renderer, "Style cannot be set because text is static and has been submitted once already.");
			return;
		}

		m_TextStyle = textStyle;
		Font::getTextBufferManager()->setStyle(m_TextBufferHandle, textStyle);
	}
#if _MSC_VER && !__INTEL_COMPILER
#pragma warning(pop)
#endif

	void Text::init(bool isStatic, bool sdf)
	{
		FontManager* fontManager = Font::getFontManager();
		TextBufferManager* textBufferManager = Font::getTextBufferManager();

		m_IsStatic = isStatic;
		m_Sdf = sdf;

		uint32_t fontType = FONT_TYPE_ALPHA;
		if (m_Sdf)
		{
			fontType = FONT_TYPE_DISTANCE;
		}

		if (isValid(m_TextBufferHandle))
		{
			Font::getTextBufferManager()->destroyTextBuffer(m_TextBufferHandle);
		}

		if (m_IsStatic)
		{
			m_TextBufferHandle = textBufferManager->createTextBuffer(fontType, BufferType::Static);
		}
		else
		{
			m_TextBufferHandle = textBufferManager->createTextBuffer(fontType, BufferType::Transient);
		}

		m_HasSubmittedOnce = false;
	}

	void Text::applyProperties()
	{
		TextBufferManager* textBufferManager = Font::getTextBufferManager();

		if (textBufferManager == nullptr || !isValid(m_TextBufferHandle) || !isValid(m_FontHandle))
		{
			ANNILEEN_LOG_ERROR(LoggingChannel::Renderer, "Properties cannot be applied because some handle is not valid.");
			return;
		}

		setPixelSize(m_PixelSize);
		setBackgroundColor(m_BackgroundColor);
		setTextColor(m_TextColor);
		setOverlineColor(m_OverlineColor);
		setUnderlineColor(m_UnderlineColor);
		setStrikeThroughColor(m_StrikeThroughColor);
		setStyle(m_TextStyle);
		textBufferManager->clearTextBuffer(m_TextBufferHandle);
		textBufferManager->setPenPosition(m_TextBufferHandle, m_ScreenPosition.x, m_ScreenPosition.y);
		textBufferManager->appendText(m_TextBufferHandle, m_FontHandle, m_Text.c_str());
	}

	void Text::render(bgfx::ViewId viewId)
	{
		TextBufferManager* textBufferManager = Font::getTextBufferManager();

		if (!m_IsStatic)
		{
			textBufferManager->clearTextBuffer(m_TextBufferHandle);
			textBufferManager->setPenPosition(m_TextBufferHandle, m_ScreenPosition.x, m_ScreenPosition.y);
			textBufferManager->appendText(m_TextBufferHandle, m_FontHandle, m_Text.c_str());
		}

		textBufferManager->submitTextBuffer(m_TextBufferHandle, viewId);

		if (!m_HasSubmittedOnce)
		{
			m_HasSubmittedOnce = true;
		}
	}

	void Text::setStatic(bool isStatic)
	{
		if (isStatic != m_IsStatic)
		{
			m_IsStatic = isStatic;
			init(m_IsStatic, m_Sdf);
			applyProperties();
		}
	}

	void Text::setSdf(bool isSdf)
	{
		if (isSdf != m_Sdf)
		{
			m_Sdf = isSdf;
			init(m_IsStatic, m_Sdf);
			applyProperties();
		}
	}


	Text::Text() : m_IsStatic(false), enabled(true), m_HasSubmittedOnce(false), m_BackgroundColor(glm::vec3(1.0f)),
		m_UnderlineColor(glm::vec3(1.0f)), m_TextColor(glm::vec3(1.0f)), m_OverlineColor(glm::vec3(1.0f)),
		m_StrikeThroughColor(glm::vec3(1.0f)), m_TextStyle(TextStyle::Normal), m_FontHandle(BGFX_INVALID_HANDLE),
		m_TextBufferHandle(BGFX_INVALID_HANDLE), m_Font(BGFX_INVALID_HANDLE), m_Sdf(false), m_ScreenPosition(glm::vec2(0.0f))
	{
		init(false);
		setFont(ServiceProvider::getAssetManager()->getFont(
			ServiceProvider::getSettings()->getData()->defaultFont)->getHandle());
	}

	Text::~Text()
	{
		if (isValid(m_FontHandle))
		{
			Font::getFontManager()->destroyFont(m_FontHandle);
		}

		if (isValid(m_TextBufferHandle))
		{
			Font::getTextBufferManager()->destroyTextBuffer(m_TextBufferHandle);
		}
	}
}
