// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <winrt/base.h>
#include "Util/Logger.h"
#include <filesystem>
#include "LanternMod.h"
#include "Lantern.h"
#include "Util/HookHelper.h"
#include "InternalHooks/Minecraft.h"

// TODO: When events happen, register internal event for mod menu uiscene

//const LANTERN_API LogFile* Lantern::logFile;
const std::string MODS_DIR = "./mods/LanternMods";

LANTERN_CLASS Lantern::Version Lantern::version = Lantern::Version(LANTERN_VERSION);

// internal hooks
std::vector<std::tuple<PVOID*, PVOID>> hooks;
std::vector<LanternMod*> mods;
std::vector<LanternMod*> enabledMods;

typedef LanternMod* (*Initialize)();
typedef uint64_t (*GetTargetedLanternVersion)();


void registerMod(const std::filesystem::directory_entry& path) {
    HINSTANCE dll = LoadLibraryW(path.path().c_str());

    if (!dll) {
        LOGW(L"Couldn't load mod ", path.path().c_str());
        return;
    }

    Initialize init = (Initialize)GetProcAddress(dll, "Initialize");
    if (!init) {
            LOGW(L"Couldn't find init function in mod ", path.path().c_str());
            return;
    }

    bool notLegacy = false;

    // we check this to make sure the mod isnt from the first versions of Lantern
    GetTargetedLanternVersion getVersion = (GetTargetedLanternVersion)GetProcAddress(dll, "GetTargetedLanternVersion");
    if (getVersion)
        notLegacy = true;

	LanternMod* mod = init();
	mods.push_back(mod);
    // TODO: Settings for both Lantern and mods.
    enabledMods.push_back(mod);

    // in the future this will be completely removed.
    if (!notLegacy) {
        LOGW_C(ANSIColor::YELLOW, L"Mod ", mod->GetName(), L" was made for an older version of Lantern and may not be supported in later versions.");
        LOGW(L"Loaded mod '", mod->GetName(), L"' v", mod->GetVersion());
    }
    else
    {
        uint64_t modLanternVersion = getVersion();

        if (modLanternVersion != Lantern::version.GetPackedVersion()) {
            LOGW_C(ANSIColor::YELLOW, L"Mod ", mod->GetName(), L" was made for a different version of Lantern (v", Lantern::Version::Version(modLanternVersion).GetVersionString(), L"). You are running Lantern v", Lantern::version.GetVersionString());
            // TODO: in Lantern settings allow loading older versions of mods if wanted
            // for now we continue
            //return;
        }

        LOGW(L"Loaded mod '", mod->GetName(), L"' v", mod->GetVersion(), L" by '", mod->GetAuthorName(), L"'");
    }

    if (std::find(enabledMods.begin(), enabledMods.end(), mod) != enabledMods.end())
	{
		mod->Enable();
	}
}

BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD callReason,
                      LPVOID lpReserved) {
    // for funny ansi shit
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    if (DetourIsHelperProcess()) {
        return TRUE;
    };

    switch (callReason)
    {
    case DLL_PROCESS_ATTACH: {
        // drog and drap, from the desktop
        winrt::init_apartment();
        LOGW_C(ANSIColor::GREEN, L"Starting Lantern v", Lantern::version.GetVersionString());

        LOG_C(ANSIColor::GREEN, "Registering internal hooks");
        registerHook(&(PVOID&)Minecraft_main, &Minecraft::main);

        LOG_C(ANSIColor::GREEN, "Loading mods");
        if (!std::filesystem::exists(MODS_DIR))
            std::filesystem::create_directories(MODS_DIR);

        for (const std::filesystem::directory_entry& f : std::filesystem::directory_iterator(MODS_DIR)) {
            if (f.is_regular_file())
                registerMod(f);
        }
        break;
    }
    case DLL_PROCESS_DETACH: {
        for (LanternMod* mod : mods) {
            mod->Disable();
        }
        break;
    }
    default: break;
    }

    return TRUE;
}

Lantern::Version::Version(std::wstring version)
{
    std::wstringstream ss(version);
    std::wstring ver;
    int verIdx = 0;

    while (std::getline(ss, ver, L'.') && verIdx < 4) {
        try {
            short value = static_cast<short>(std::stoi(ver));

            switch (verIdx) {
                case 0: major = value; break;
                case 1: minor = value; break;
                case 2: build = value; break;
                case 3: patch = value; break;
            }
        }
        catch (const std::invalid_argument& e) {
            LOGW(L"Invalid number in version string");
        }
        catch (const std::out_of_range& e) {
            LOGW(L"Version number too big for a short");
        }
        catch (const std::exception& e) {
            LOGW(L"Couldn't parse ver string: ", e.what());
        }
        verIdx++;
    }
}

Lantern::Version::Version(uint16_t major, uint16_t minor, uint16_t build, uint16_t patch) : major(major), minor(minor), build(build), patch(patch) {}

Lantern::Version::Version(uint64_t packed)
{
	major = packed >> 48;
	minor = (packed >> 32) & 0xFFFF;
	build = (packed >> 16) & 0xFFFF;
	patch = packed & 0xFFFF;
}

uint64_t Lantern::Version::GetPackedVersion()
{
    return ((uint64_t)major << 48) | ((uint64_t)minor << 32) | ((uint64_t)build << 16) | (uint64_t)patch;
}

std::wstring Lantern::Version::GetVersionString()
{
    std::wstringstream ss;
    ss << major << L"." << minor << L"." << build << L"." << patch;
    return ss.str();
}

