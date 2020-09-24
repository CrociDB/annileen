#include <engine/text/text.h>
#include <engine/serviceprovider.h>
#include <engine/engine.h>

namespace annileen
{
	void Text::createFont()
	{
		FontManager* fontManager = ServiceProvider::getFontManager();
		
		// If font is not defined or valid, use font default.
		if (!isValid(m_Font))
		{
			m_Font = ServiceProvider::getAssetManager()->loadFont(
				ServiceProvider::getSettings()->getFontDefault())->getHandle();
		}

		if (isValid(m_FontHandle))
		{
			fontManager->destroyFont(m_FontHandle);
		}

		if (m_Sdf)
		{
			m_FontHandle = fontManager->createFontByPixelSize(m_Font, 0, m_PixelSize, FONT_TYPE_DISTANCE);
		}
		else
		{
			m_FontHandle = fontManager->createFontByPixelSize(m_Font, 0, m_PixelSize);
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
		ServiceProvider::getTextBufferManager()->clearTextBuffer(m_TextBufferHandle);
		ServiceProvider::getTextBufferManager()->setPenPosition(m_TextBufferHandle, m_ScreenPosition.x, m_ScreenPosition.y);
		ServiceProvider::getTextBufferManager()->appendText(m_TextBufferHandle, m_FontHandle, text.c_str());
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
		ServiceProvider::getTextBufferManager()->setPenPosition(m_TextBufferHandle, x, y);
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
		ServiceProvider::getTextBufferManager()->setBackgroundColor(m_TextBufferHandle, convert_color_vec3_uint32(backgroundColor));
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
		ServiceProvider::getTextBufferManager()->setTextColor(m_TextBufferHandle, convert_color_vec3_uint32(textColor));
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
		ServiceProvider::getTextBufferManager()->setUnderlineColor(m_TextBufferHandle, convert_color_vec3_uint32(underlineColor));
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
		ServiceProvider::getTextBufferManager()->setOverlineColor(m_TextBufferHandle, convert_color_vec3_uint32(overlineColor));
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
		ServiceProvider::getTextBufferManager()->setStrikeThroughColor(m_TextBufferHandle, convert_color_vec3_uint32(strikeThroughColor));
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
		ServiceProvider::getTextBufferManager()->setStyle(m_TextBufferHandle, textStyle);
	}
#if _MSC_VER && !__INTEL_COMPILER
#pragma warning(pop)
#endif

	void Text::init(bool isStatic, bool sdf)
	{
		FontManager* fontManager = ServiceProvider::getFontManager();
		TextBufferManager* textBufferManager = ServiceProvider::getTextBufferManager();

		m_IsStatic = isStatic;
		m_Sdf = sdf;

		uint32_t fontType = FONT_TYPE_ALPHA;
		if (m_Sdf)
		{
			fontType = FONT_TYPE_DISTANCE;
		}

		if (isValid(m_TextBufferHandle))
		{
			ServiceProvider::getTextBufferManager()->destroyTextBuffer(m_TextBufferHandle);
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
		TextBufferManager* textBufferManager = ServiceProvider::getTextBufferManager();

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
		setFont(ServiceProvider::getAssetManager()->loadFont(
			ServiceProvider::getSettings()->getFontDefault())->getHandle());
	}

	Text::~Text()
	{
		if (isValid(m_FontHandle))
		{
			ServiceProvider::getFontManager()->destroyFont(m_FontHandle);
		}

		if (isValid(m_TextBufferHandle))
		{
			ServiceProvider::getTextBufferManager()->destroyTextBuffer(m_TextBufferHandle);
		}
	}
}