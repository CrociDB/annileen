module;

export module serviceprovider;

//import audio;
import assetmanager;
import settings;
import scenemanager;

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
		static SceneManager* s_SceneManager;

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

		static void provideSceneManager(SceneManager* sceneManager);
		static SceneManager* getSceneManager();
	};
}

namespace annileen
{
	AssetManager* ServiceProvider::s_AssetManagerService = nullptr;
	Settings* ServiceProvider::s_SettingsService = nullptr;
	SceneManager* ServiceProvider::s_SceneManager = nullptr;

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

	void ServiceProvider::provideSceneManager(SceneManager* sceneManager)
	{
		if (s_SceneManager != nullptr)
		{
			delete s_SceneManager;
		}

		s_SceneManager = sceneManager;
	}
	
	SceneManager* ServiceProvider::getSceneManager()
	{
		return s_SceneManager;
	}
}
