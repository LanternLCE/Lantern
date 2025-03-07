// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <winrt/base.h>
#include "Util/Logger.h"
#include <filesystem>
#include "LanternMod.h"
#include "Lantern.h"
#include "Util/HookHelper.h"
#include "InternalHooks/Minecraft.h"

const std::string MODS_DIR = "./mods/LanternMods";
std::vector<std::unique_ptr<LanternMod>> mods;

typedef LanternMod* (*Initialize)();

void registerMod(const std::filesystem::directory_entry& path) {
    HINSTANCE dll = LoadLibraryW(path.path().c_str());
    if (!dll) {
        LOGW(L"Failed to load mod: ", path.path().c_str());
        return;
    }

    Initialize init = reinterpret_cast<Initialize>(GetProcAddress(dll, "Initialize"));
    if (!init) {
        LOGW(L"Missing Initialize function in mod: ", path.path().c_str());
        FreeLibrary(dll);
        return;
    }

    auto mod = std::unique_ptr<LanternMod>(init());
    LOGW(L"Loaded mod '", mod->GetName(), L"' v", mod->GetVersion());
    mod->Enable();  
    mods.push_back(std::move(mod));
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD callReason, LPVOID lpReserved) {
    if (callReason == DLL_PROCESS_ATTACH) {
        winrt::init_apartment();
        LOG_C(ANSIColor::GREEN, "Starting Lantern");

        LOG_C(ANSIColor::GREEN, "Registering internal hooks");
        registerHook(&(PVOID&)Minecraft_main, &Minecraft::main);

        LOG_C(ANSIColor::GREEN, "Loading mods");
        if (!std::filesystem::exists(MODS_DIR)) {
            std::filesystem::create_directories(MODS_DIR);
        }

        for (const auto& f : std::filesystem::directory_iterator(MODS_DIR)) {
            if (f.is_regular_file()) registerMod(f);
        }
    } 
    else if (callReason == DLL_PROCESS_DETACH) {
        for (auto& mod : mods) {
            mod->Disable();
        }
    }
    return TRUE;
}

void Lantern::IgnoreMe() {}
