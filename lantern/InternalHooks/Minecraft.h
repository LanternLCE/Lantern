#pragma once
#include <Psapi.h>
#include <iostream>
#include <cstdio>
#include <cstdint>
#include "Util/Logger.h"
#include "Util/HookHelper.h"
#include "Util/Event.h"

#ifdef LANTERN_EXPORTS
#define MINECRAFT_EVENT __declspec(dllexport)
#else  
#define MINECRAFT_EVENT __declspec(dllimport)
#endif

class Minecraft {
public:
	/// <summary>
	/// An event provided for mods to initialize things that depend on the game having started.
	/// 
	/// For example, trying to access UWP before main has been called does not work.
	/// </summary>
	static MINECRAFT_EVENT Event* onMain;

	static int main(int _Argc, char** _Argv, char** _Env);
};

CREATE_FUNC(Minecraft_main, 0x1408a7570, int, int _Argc, char** _Argv, char** _Env);