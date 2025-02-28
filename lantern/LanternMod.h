#pragma once

#ifdef LANTERN_EXPORTS
#define LANTERNMOD_CLASS __declspec(dllexport)
#else  
#define LANTERNMOD_CLASS __declspec(dllimport)
#endif

#include <string>
class LanternMod {
public:
	virtual void Enable() = 0;
	virtual void Disable() = 0;
	virtual std::wstring GetVersion() = 0;
	virtual std::wstring GetName() = 0;
};