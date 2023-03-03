module;

#include <engine/text/fontmanager.h>
#include <engine/text/textbuffermanager.h>
#include <engine/core/logger.h>
#include <glm.hpp>
#include <string>
#include <iostream>

export module text;

import serviceprovider;
import utils;
import scenenode;
import font;

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

		Text();
		~Text();

		void createFont();

		void init(bool isStatic, bool sdf = false);
		void applyProperties();
		
		void render(bgfx::ViewId viewId);

		void setFont(const TrueTypeHandle& font);
		
		void setStatic(bool isStatic);
		bool isStatic() const noexcept;		
		
		void setSdf(bool isSdf);
		bool isSdf() const noexcept;

		void setPixelSize(uint32_t pixelSize);
		uint32_t getPixelSize() const noexcept;

		void setText(const std::string& text);
		const std::string& getText() const noexcept;

		void setScreenPosition(float x, float y);
		const glm::vec2& getScreenPosition() const noexcept;

		void setBackgroundColor(const glm::vec3& backgroundColor);
		const glm::vec3& getBackgroundColor() const noexcept;

		void setTextColor(const glm::vec3& textColor);
		const glm::vec3& getTextColor() const noexcept;

		void setUnderlineColor(const glm::vec3& underlineColor);
		const glm::vec3& getUnderlineColor() const noexcept;

		void setOverlineColor(const glm::vec3& overlineColor);
		const glm::vec3& getOverlineColor() const noexcept;

		void setStrikeThroughColor(const glm::vec3& strikeThroughColor);
		const glm::vec3& getStrikeThroughColor() const noexcept;

		void setStyle(const TextStyle& textStyle);
		const TextStyle& getStyle() const noexcept;

	public:
		bool enabled{ true };

	private:
		uint32_t m_PixelSize{ 32 };
		TrueTypeHandle m_Font{ BGFX_INVALID_HANDLE };
		FontHandle m_FontHandle{ BGFX_INVALID_HANDLE };
		TextBufferHandle m_TextBufferHandle{ BGFX_INVALID_HANDLE };
		std::string m_Text{""};
		bool m_Sdf{ false };
		bool m_IsStatic{ false };
		bool m_HasSubmittedOnce{ false };

		glm::vec2 m_ScreenPosition{ glm::vec2(0.0f) };
		glm::vec3 m_BackgroundColor{ glm::vec3(1.0f) };
		glm::vec3 m_TextColor{ glm::vec3(1.0f) };
		glm::vec3 m_UnderlineColor{ glm::vec3(1.0f) };
		glm::vec3 m_OverlineColor{ glm::vec3(1.0f) };
		glm::vec3 m_StrikeThroughColor{ glm::vec3(1.0f) };

		TextStyle m_TextStyle{ TextStyle::Normal };
	};
}

module :private;

namespace annileen
{
	Text::Text()
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

		// TODO: remove
		std::cout << "Text destroyed." << std::endl;
	}

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

	void Text::setFont(const TrueTypeHandle& font)
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

	uint32_t Text::getPixelSize() const noexcept 
	{ 
		return m_PixelSize; 
	}

	void Text::setText(const std::string& text)
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

	const std::string& Text::getText() const noexcept 
	{ 
		return m_Text; 
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

	const glm::vec2& Text::getScreenPosition() const noexcept 
	{ 
		return m_ScreenPosition; 
	}

	void Text::setBackgroundColor(const glm::vec3& backgroundColor)
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

	const glm::vec3& Text::getBackgroundColor() const noexcept 
	{ 
		return m_BackgroundColor; 
	}

	void Text::setTextColor(const glm::vec3& textColor)
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

	const glm::vec3& Text::getTextColor() const noexcept 
	{ 
		return m_TextColor; 
	}

	void Text::setUnderlineColor(const glm::vec3& underlineColor)
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

	const glm::vec3& Text::getUnderlineColor() const noexcept
	{
		return m_UnderlineColor;
	}

	void Text::setOverlineColor(const glm::vec3& overlineColor)
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

	const glm::vec3& Text::getOverlineColor() const noexcept
	{
		return m_OverlineColor;
	}

	void Text::setStrikeThroughColor(const glm::vec3& strikeThroughColor)
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

	const glm::vec3& Text::getStrikeThroughColor() const noexcept 
	{ 
		return m_StrikeThroughColor; 
	}

#if _MSC_VER && !__INTEL_COMPILER
#pragma warning(push)
#pragma warning(disable : 26812) 
#endif
	void Text::setStyle(const Text::TextStyle& textStyle)
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

	const Text::TextStyle& Text::getStyle() const noexcept 
	{ 
		return m_TextStyle; 
	}

	void Text::init(bool isStatic, bool sdf)
	{
		auto fontManager{ Font::getFontManager() };
		auto textBufferManager{ Font::getTextBufferManager() };

		m_IsStatic = isStatic;
		m_Sdf = sdf;

		uint32_t fontType{ FONT_TYPE_ALPHA };
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
		auto textBufferManager{ Font::getTextBufferManager() };

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
		auto textBufferManager{ Font::getTextBufferManager() };

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

	bool Text::isStatic() const noexcept
	{
		return m_IsStatic;
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

	bool Text::isSdf() const noexcept
	{
		return m_Sdf;
	}
}
