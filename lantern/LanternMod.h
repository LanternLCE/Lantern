#pragma once

#ifdef LANTERN_EXPORTS
#define LANTERNMOD_CLASS __declspec(dllexport)
#else  
#define LANTERNMOD_CLASS __declspec(dllimport)
#endif

#include <string>
#include <Lantern.h>
class LanternMod {
public:
	/// <summary>
	/// Runs when the mod is enabled
	/// </summary>
	virtual void Enable() = 0;
	/// <summary>
	/// Runs when the mod is disabled
	/// </summary>
	virtual void Disable() = 0;
	// TODO: maybe replace with Version class?
	/// <summary>
	/// Gets the mod version
	/// </summary>
	virtual std::wstring GetVersion() {
		return L"0.0.0";
	};
	/// <summary>
	/// Gets the mod name.
	/// If none is present, it returns the mod ID.
	/// </summary>
	virtual std::wstring GetName() {
		return GetID();
	};
	/// <summary>
	/// Gets the mod description.
	/// </summary>
	virtual std::wstring GetDescription() {
		return L"";
	};
	/// <summary>
	/// Gets the mod author
	/// </summary>
	virtual std::wstring GetAuthorName() {
		return L"Unknown";
	};
	/// <summary>
	/// Gets the mod ID
	/// </summary>
	virtual std::wstring GetID() {
		return L"lantern_default_mod_id_changeme";
	};
	/// <summary>
	/// Gets the version of Lantern the mod was made for
	/// Overriding is discouraged as the Lantern version is already set when you build a mod.
	/// </summary>
	virtual uint64_t GetTargetedLanternVersion() {
		return Lantern::Version::Version(LANTERN_VERSION).GetPackedVersion();
	};
};