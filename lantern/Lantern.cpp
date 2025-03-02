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

// internal hooks
std::vector<std::tuple<PVOID*, PVOID>> hooks;
std::vector<LanternMod*> mods;
std::vector<LanternMod*> enabledMods;

typedef LanternMod* (*Initialize)();

void registerMod(const std::filesystem::directory_entry& path) {
    HINSTANCE dll = LoadLibrary(path.path().c_str());

    if (!dll) {
        LOGW(L"Couldn't load mod ", path.path().c_str());
        return;
    }

	Initialize init = (Initialize)GetProcAddress(dll, "Initialize");
	if (!init) {
        LOGW(L"Couldn't find init function in mod ", path.path().c_str());
        return;
	}

	LanternMod* mod = init();
	mods.push_back(mod);
    // TODO: Settings for both Lantern and mods.
    enabledMods.push_back(mod);

    LOGW(L"Loaded mod '", mod->GetName(), L"' v", mod->GetVersion());

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
        LOG_C(ANSIColor::GREEN, "Starting Lantern");

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

void Lantern::IgnoreMe()
{
}
