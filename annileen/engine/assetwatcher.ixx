module;

#include <filesystem>
#include <chrono>
#include <unordered_map>
#include <functional>
#include <tuple>
#include <vector>
#include <iostream>

export module assetwatcher;

export namespace annileen
{
	enum class AssetFileStatus
	{
		Created,
		Modified,
		Erased
	};

	class AssetWatcher
	{
	public:
		AssetWatcher(const std::string& path);
		~AssetWatcher();

		void update();
		inline std::unordered_map<std::string, AssetFileStatus>& getModified() { return m_Modified; }
		inline bool hasModified() const { return m_Modified.size() > 0; }
		inline void resetModified() { m_Modified.clear(); }

	private:
		inline bool contains(const std::string& filename) const
		{
			return m_AssetFiles.find(filename) != m_AssetFiles.end();
		}

		std::string m_Path{ "" };
		std::unordered_map<std::string, std::filesystem::file_time_type> m_AssetFiles{};
		std::unordered_map<std::string, AssetFileStatus> m_Modified{};
	};
}

namespace annileen
{
	AssetWatcher::AssetWatcher(const std::string& path) :
		m_Path(path)
	{
		for (auto& file : std::filesystem::recursive_directory_iterator(m_Path))
		{
			m_AssetFiles[file.path().string()] = std::filesystem::last_write_time(file);
		}
	}

	AssetWatcher::~AssetWatcher()
	{
		// TODO: remove
		std::cout << "AssetWatcher destroyed." << std::endl;
	}

	void AssetWatcher::update()
	{
		// Lazy frame skipper
		static int framecounter{ 0 };
		if (framecounter++ < 60) return;
		else framecounter = 0;

		// Check erased files
		auto it{ m_AssetFiles.begin() };
		while (it != m_AssetFiles.end())
		{
			if (!std::filesystem::exists(it->first))
			{
				m_Modified[it->first] = AssetFileStatus::Erased;
				it = m_AssetFiles.erase(it);
			}
			else
			{
				it++;
			}
		}

		// Check modified and new files
		for (auto& file : std::filesystem::recursive_directory_iterator(m_Path))
		{
			auto last_write_time{ std::filesystem::last_write_time(file) };
			auto filename{ file.path().string() };

			if (!contains(filename))
			{
				m_AssetFiles[filename] = last_write_time;
				m_Modified[filename] = AssetFileStatus::Created;
			}
			else
			{
				if (m_AssetFiles[filename] != last_write_time)
				{
					m_AssetFiles[filename] = last_write_time;
					m_Modified[filename] = AssetFileStatus::Modified;
				}
			}
		}
	}
}
