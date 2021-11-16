#include "assetwatcher.h"

annileen::AssetWatcher::AssetWatcher(const std::string& path) :
	m_Path(path)
{
	for (auto& file : std::filesystem::recursive_directory_iterator(m_Path)) 
	{
		m_AssetFiles[file.path().string()] = std::filesystem::last_write_time(file);
	}
}

void annileen::AssetWatcher::update()
{
	// Lazy frame skipper
	static int framecounter = 0;
	if (framecounter++ < 60) return;
	else framecounter = 0;

	// Check erased files
	auto it = m_AssetFiles.begin();
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
		auto last_write_time = std::filesystem::last_write_time(file);
		auto filename = file.path().string();

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
