#pragma once

#include <filesystem>
#include <chrono>
#include <unordered_map>
#include <functional>
#include <tuple>
#include <vector>

namespace annileen
{
	enum class AssetFileStatus
	{
		Created,
		Modified,
		Erased
	};

	class AssetWatcher
	{
	private:
		std::string m_Path;
		std::unordered_map<std::string, std::filesystem::file_time_type> m_AssetFiles;
		std::unordered_map<std::string, AssetFileStatus> m_Modified;

		inline bool contains(const std::string& filename)
		{
			return m_AssetFiles.find(filename) != m_AssetFiles.end();
		}

	public:
		AssetWatcher(const std::string& path);
		void update();

		inline std::unordered_map<std::string, AssetFileStatus>& getModified() { return m_Modified; }
		inline bool hasModified() { return m_Modified.size() > 0; }
		inline void resetModified() { m_Modified.clear(); }
	};
}
