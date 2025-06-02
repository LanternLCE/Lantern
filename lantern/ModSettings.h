#include <string>
#include <Windows.h>
#include <wrl.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Core.h>
#include <windows.storage.h>

class ModSettings {
	std::filesystem::path settingsPath;

public:
	ModSettings(std::wstring name) {
        winrt::Windows::Storage::StorageFolder folder = (winrt::Windows::Storage::ApplicationData::Current()).LocalFolder();
        std::filesystem::path lanternDir = std::filesystem::path(folder.Path().c_str()) / "Lantern";

		std::filesystem::path configPath = lanternDir / "config";
		if (!std::filesystem::exists(configPath))
			std::filesystem::create_directories(configPath);

		settingsPath = configPath / (name + L".yml");
		if (!std::filesystem::exists(settingsPath)) {
			std::ofstream file(settingsPath);
			file.close();
		}
	};
};
