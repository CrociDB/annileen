#include <engine/text/text.h>

#include <bx/allocator.h>

namespace annileen
{
	bx::FileReaderI* Text::s_fileReader = nullptr;
	bx::AllocatorI* Text::g_allocator = &s_allocator;
	bx::DefaultAllocator Text::s_allocator;
	Text::String Text::s_currentDir;


	void Text::init()
	{
		// Init the text rendering system.
		m_fontManager = new FontManager(512);
		m_textBufferManager = new TextBufferManager(m_fontManager);

		// Load some TTF files.
		for (uint32_t ii = 0; ii < numFonts; ++ii)
		{
			// Instantiate a usable font.
			m_fontFiles[ii] = loadTtf(m_fontManager, s_fontFilePath[ii]);
			m_fonts[ii] = m_fontManager->createFontByPixelSize(m_fontFiles[ii], 0, 32);

			// Preload glyphs and blit them to atlas.
			m_fontManager->preloadGlyph(m_fonts[ii], L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ. \n");

			// You can unload the truetype files at this stage, but in that
			// case, the set of glyph's will be limited to the set of preloaded
			// glyph.
			m_fontManager->destroyTtf(m_fontFiles[ii]);
		}

		m_fontAwesomeTtf = loadTtf(m_fontManager, "assets/font/fontawesome-webfont.ttf");
		m_fontKenneyTtf = loadTtf(m_fontManager, "assets/font/kenney-icon-font.ttf");

		// This font doesn't have any preloaded glyph's but the truetype file
	// is loaded so glyph will be generated as needed.
		m_fontAwesome72 = m_fontManager->createFontByPixelSize(m_fontAwesomeTtf, 0, 72);
		m_fontKenney64 = m_fontManager->createFontByPixelSize(m_fontKenneyTtf, 0, 64);

		m_visitorTtf = loadTtf(m_fontManager, "assets/font/visitor1.ttf");

		// This font doesn't have any preloaded glyph's but the truetype file
		// is loaded so glyph will be generated as needed.
		m_visitor10 = m_fontManager->createFontByPixelSize(m_visitorTtf, 0, 10);

		//create a static text buffer compatible with alpha font
		//a static text buffer content cannot be modified after its first submit.
		m_staticText = m_textBufferManager->createTextBuffer(FONT_TYPE_ALPHA, BufferType::Static);

		// The pen position represent the top left of the box of the first line
		// of text.
		m_textBufferManager->setPenPosition(m_staticText, 24.0f, 100.0f);

		for (uint32_t ii = 0; ii < numFonts; ++ii)
		{
			// Add some text to the buffer.
			// The position of the pen is adjusted when there is an endline.
			m_textBufferManager->appendText(m_staticText, m_fonts[ii], L"The quick brown fox jumps over the lazy dog\n");
		}

		// Now write some styled text.

		// Setup style colors.
		m_textBufferManager->setBackgroundColor(m_staticText, 0x551111ff);
		m_textBufferManager->setUnderlineColor(m_staticText, 0xff2222ff);
		m_textBufferManager->setOverlineColor(m_staticText, 0x2222ffff);
		m_textBufferManager->setStrikeThroughColor(m_staticText, 0x22ff22ff);

		// Background.
		m_textBufferManager->setStyle(m_staticText, STYLE_BACKGROUND);
		m_textBufferManager->appendText(m_staticText, m_fonts[0], L"The quick ");

		// Strike-through.
		m_textBufferManager->setStyle(m_staticText, STYLE_STRIKE_THROUGH);
		m_textBufferManager->appendText(m_staticText, m_fonts[0], L"brown fox ");

		// Overline.
		m_textBufferManager->setStyle(m_staticText, STYLE_OVERLINE);
		m_textBufferManager->appendText(m_staticText, m_fonts[0], L"jumps over ");

		// Underline.
		m_textBufferManager->setStyle(m_staticText, STYLE_UNDERLINE);
		m_textBufferManager->appendText(m_staticText, m_fonts[0], L"the lazy ");

		// Background + strike-through.
		m_textBufferManager->setStyle(m_staticText, STYLE_BACKGROUND | STYLE_STRIKE_THROUGH);
		m_textBufferManager->appendText(m_staticText, m_fonts[0], L"dog\n");

		/*m_textBufferManager->setStyle(m_staticText, STYLE_NORMAL);
		m_textBufferManager->appendText(m_staticText, m_fontAwesome72,
			" " ICON_FA_POWER_OFF
			" " ICON_FA_TWITTER_SQUARE
			" " ICON_FA_CERTIFICATE
			" " ICON_FA_FLOPPY_O
			" " ICON_FA_GITHUB
			" " ICON_FA_GITHUB_ALT
			"\n"
		);
		m_textBufferManager->appendText(m_staticText, m_fontKenney64,
			" " ICON_KI_COMPUTER
			" " ICON_KI_JOYSTICK
			" " ICON_KI_EXLAMATION
			" " ICON_KI_STAR
			" " ICON_KI_BUTTON_START
			" " ICON_KI_DOWNLOAD
			"\n"
		);*/

		// Create a transient buffer for real-time data.
		m_transientText = m_textBufferManager->createTextBuffer(FONT_TYPE_ALPHA, BufferType::Transient);		
	}

	void Text::shutdown()
	{
		m_fontManager->destroyTtf(m_fontKenneyTtf);
		m_fontManager->destroyTtf(m_fontAwesomeTtf);
		m_fontManager->destroyTtf(m_visitorTtf);

		// Destroy the fonts.
		m_fontManager->destroyFont(m_fontKenney64);
		m_fontManager->destroyFont(m_fontAwesome72);
		m_fontManager->destroyFont(m_visitor10);
		for (uint32_t ii = 0; ii < numFonts; ++ii)
		{
			m_fontManager->destroyFont(m_fonts[ii]);
		}

		m_textBufferManager->destroyTextBuffer(m_staticText);
		m_textBufferManager->destroyTextBuffer(m_transientText);

		delete m_textBufferManager;
		delete m_fontManager;
	}

	void Text::update(bgfx::ViewId viewId, uint16_t width, uint16_t height)
	{
		m_textBufferManager->clearTextBuffer(m_transientText);
		m_textBufferManager->setPenPosition(m_transientText, width - 150.0f, 100.0f);
		m_textBufferManager->appendText(m_transientText, m_visitor10, "Transient\n");
		m_textBufferManager->appendText(m_transientText, m_visitor10, "text buffer\n");


		// Submit the debug text.
		m_textBufferManager->submitTextBuffer(m_transientText, viewId);

		// Submit the static text.
		m_textBufferManager->submitTextBuffer(m_staticText, viewId);
	}

	Text::Text()
	{
		init();
	}

	Text::~Text()
	{
		shutdown();
	}
}