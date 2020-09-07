#include <engine/text/text.h>
#include <engine/fontloader.h>
#include <engine/serviceprovider.h>
#include <engine/engine.h>

namespace annileen
{
	void Text::createFont()
	{
		FontManager* fontManager = ServiceProvider::getFontManager();
		
		// TODO: add fallback font.

		//if (isValid(m_FontHandle))
		//{
		//	fontManager->destroyFont(m_FontHandle);
		//}

		m_FontHandle = fontManager->createFontByPixelSize(m_Font, 0, m_PixelSize);

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
		if (m_IsStatic && m_HasSubmittedOnce)
		{
			ANNILEEN_LOG_WARNING(LoggingChannel::Renderer, "Text cannot be set because text is static and has been submitted once already.");
			return;
		}

		m_Text = text;
		ServiceProvider::getTextBufferManager()->clearTextBuffer(m_TextBufferHandle);
		ServiceProvider::getTextBufferManager()->setPenPosition(m_TextBufferHandle, m_ScreenPosition.x, m_ScreenPosition.y);
		ServiceProvider::getTextBufferManager()->appendText(m_TextBufferHandle, m_FontHandle, text.c_str());
	}

	void Text::setScreenPosition(float x, float y)
	{
		if (m_IsStatic && m_HasSubmittedOnce)
		{
			ANNILEEN_LOG_WARNING(LoggingChannel::Renderer, "New position cannot be set because text is static and has been submitted once already.");
			return;
		}

		m_ScreenPosition = glm::vec2(x, y);
		ServiceProvider::getTextBufferManager()->setPenPosition(m_TextBufferHandle, x, y);
	}

	void Text::setBackgroundColor(glm::vec3 backgroundColor)
	{
		if (m_IsStatic && m_HasSubmittedOnce)
		{
			ANNILEEN_LOG_WARNING(LoggingChannel::Renderer, "Color cannot be set because text is static and has been submitted once already.");
			return;
		}

		m_BackgroundColor = backgroundColor;
		ServiceProvider::getTextBufferManager()->setBackgroundColor(m_TextBufferHandle, convert_color_vec3_uint32(backgroundColor));
	}

	void Text::setTextColor(glm::vec3 textColor)
	{
		if (m_IsStatic && m_HasSubmittedOnce)
		{
			ANNILEEN_LOG_WARNING(LoggingChannel::Renderer, "Color cannot be set because text is static and has been submitted once already.");
			return;
		}

		m_TextColor = textColor;
		ServiceProvider::getTextBufferManager()->setTextColor(m_TextBufferHandle, convert_color_vec3_uint32(textColor));
	}

	void Text::setUnderlineColor(glm::vec3 underlineColor)
	{
		if (m_IsStatic && m_HasSubmittedOnce)
		{
			ANNILEEN_LOG_WARNING(LoggingChannel::Renderer, "Color cannot be set because text is static and has been submitted once already.");
			return;
		}

		m_UnderlineColor = underlineColor;
		ServiceProvider::getTextBufferManager()->setUnderlineColor(m_TextBufferHandle, convert_color_vec3_uint32(underlineColor));
	}

	void Text::setOverlineColor(glm::vec3 overlineColor)
	{
		if (m_IsStatic && m_HasSubmittedOnce)
		{
			ANNILEEN_LOG_WARNING(LoggingChannel::Renderer, "Color cannot be set because text is static and has been submitted once already.");
			return;
		}

		m_OverlineColor = overlineColor;
		ServiceProvider::getTextBufferManager()->setOverlineColor(m_TextBufferHandle, convert_color_vec3_uint32(overlineColor));
	}

	void Text::setStrikeThroughColor(glm::vec3 strikeThroughColor)
	{
		if (m_IsStatic && m_HasSubmittedOnce)
		{
			ANNILEEN_LOG_WARNING(LoggingChannel::Renderer, "Color cannot be set because text is static and has been submitted once already.");
			return;
		}

		m_StrikeThroughColor = strikeThroughColor;
		ServiceProvider::getTextBufferManager()->setStrikeThroughColor(m_TextBufferHandle, convert_color_vec3_uint32(strikeThroughColor));
	}

#if _MSC_VER && !__INTEL_COMPILER
#pragma warning(push)
#pragma warning(disable : 26812) 
#endif
	void Text::setStyle(TextStyle textStyle)
	{
		if (m_IsStatic && m_HasSubmittedOnce)
		{
			ANNILEEN_LOG_WARNING(LoggingChannel::Renderer, "Style cannot be set because text is static and has been submitted once already.");
			return;
		}

		ServiceProvider::getTextBufferManager()->setStyle(m_TextBufferHandle, textStyle);
	}
#if _MSC_VER && !__INTEL_COMPILER
#pragma warning(pop)
#endif

	void Text::init(bool isStatic)
	{
		FontManager* fontManager = ServiceProvider::getFontManager();
		TextBufferManager* textBufferManager = ServiceProvider::getTextBufferManager();

		m_IsStatic = isStatic;

		if (m_IsStatic)
		{
			m_TextBufferHandle = textBufferManager->createTextBuffer(FONT_TYPE_ALPHA, BufferType::Static);
		}
		else
		{
			m_TextBufferHandle = textBufferManager->createTextBuffer(FONT_TYPE_ALPHA, BufferType::Transient);
		}
	}

	void Text::render(bgfx::ViewId viewId)
	{
		TextBufferManager* textBufferManager = ServiceProvider::getTextBufferManager();

		if (!m_IsStatic)
		{
			textBufferManager->clearTextBuffer(m_TextBufferHandle);
			textBufferManager->setPenPosition(m_TextBufferHandle, m_ScreenPosition.x, m_ScreenPosition.y);
			textBufferManager->appendText(m_TextBufferHandle, m_FontHandle, m_Text.c_str());
		} 

		textBufferManager->submitTextBuffer(m_TextBufferHandle, viewId);

		if(!m_HasSubmittedOnce)
		{ 
			m_HasSubmittedOnce = true;
		}
	}

	Text::Text() : m_IsStatic(false), enabled(true), m_HasSubmittedOnce(false), m_BackgroundColor(glm::vec3(1.0f)),
		m_UnderlineColor(glm::vec3(1.0f)), m_TextColor(glm::vec3(1.0f)), m_OverlineColor(glm::vec3(1.0f)),
		m_StrikeThroughColor(glm::vec3(1.0f)), m_TextStyle(TextStyle::Normal)
	{
	}

	Text::~Text()
	{
		ServiceProvider::getFontManager()->destroyFont(m_FontHandle);
		ServiceProvider::getTextBufferManager()->destroyTextBuffer(m_TextBufferHandle);
	}
}