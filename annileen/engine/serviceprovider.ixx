module;

export module serviceprovider;

//import audio;
import assetmanager;
import settings;

export namespace annileen
{
	class ServiceProvider final
	{
	private:
		~ServiceProvider() = default;
		ServiceProvider() = default;

		//static Audio* m_audioService;
		static AssetManager* s_AssetManagerService;
		static Settings* s_SettingsService;

	public:
		ServiceProvider(const ServiceProvider&) = delete;
		ServiceProvider& operator=(const ServiceProvider&) = delete;
		ServiceProvider(ServiceProvider&&) = delete;
		ServiceProvider& operator=(ServiceProvider&&) = delete;

		//static void initialize();

		//static void provideAudio(Audio* audio);
		//static Audio* getAudio();

		static void provideAssetManager(AssetManager* assetManager);
		static AssetManager* getAssetManager();

		static void provideSettings(Settings* settings);
		static Settings* getSettings();
	};
}

module :private;

namespace annileen
{
	AssetManager* ServiceProvider::s_AssetManagerService = nullptr;
	Settings* ServiceProvider::s_SettingsService = nullptr;

	void ServiceProvider::provideAssetManager(AssetManager* assetManager)
	{
		s_AssetManagerService = assetManager;
	}

	AssetManager* ServiceProvider::getAssetManager()
	{
		return s_AssetManagerService;
	}

	void ServiceProvider::provideSettings(Settings* settings)
	{
		s_SettingsService = settings;
	}

	Settings* ServiceProvider::getSettings()
	{
		return s_SettingsService;
	}
}
