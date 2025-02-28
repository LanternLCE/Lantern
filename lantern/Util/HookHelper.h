#pragma once
#include <vector>
#include "framework.h"

#ifdef LANTERN_EXPORTS
#define ADDRHELPER_API __declspec(dllexport)
#else  
#define ADDRHELPER_API __declspec(dllimport)
#endif

// TODO: Somehow make it more clear that they need to make this, or find a different way to store a hook registry
extern std::vector<std::tuple<PVOID*, PVOID>> hooks;

/// Gets the address within memory from the executable's address
#define GET_ADDRESS(addr) ((reinterpret_cast<size_t>(GetModuleHandleW(NULL))) + ((addr) - 0x140000000))
/// Gets the address within the executable from a memory address
#define GET_ORIG_ADDRESS(addr) (((addr) - reinterpret_cast<size_t>(GetModuleHandleW(NULL))) + 0x140000000)
/// <summary>
/// Defines a function that can be called and hooked into.
/// </summary>
/// <param name="name">The name that you want to use when referring to the original game function</param>
/// <param name="addr">The address of the function relative to the program</param>
/// <param name="returnType">The return type of the function</param>
/// <param name="...">The arguments of the function</param>
#define CREATE_FUNC(name, addr, returnType, ...) \
    using t##name = returnType(__fastcall*)(__VA_ARGS__); \
    inline t##name name = reinterpret_cast<t##name>(GET_ADDRESS(addr));

using VTable = void(*)();

/// <summary>
/// Registers a hook
/// </summary>
/// <param name="orig">The original function found within MC, defined using CREATE_FUNC</param>
/// <param name="hook">The hook/replacement function, has to be static.</param>
/// <returns></returns>
inline LONG registerHook(PVOID* orig, PVOID hook) {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    LONG att = DetourAttach(orig, hook);
    hooks.push_back(std::tuple(orig, hook));
    DetourTransactionCommit();
    return att;
}

/// <summary>
/// Unregisters a hook
/// </summary>
/// <param name="orig">The original function found within MC, defined using CREATE_FUNC</param>
/// <param name="hook">The hook/replacement function, has to be static.</param>
/// <returns></returns>
inline LONG unregisterHook(PVOID* orig, PVOID hook) {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    LONG att = DetourDetach(orig, hook);
    hooks.erase(std::remove(hooks.begin(), hooks.end(), std::tuple(orig, hook)), hooks.end());
    DetourTransactionCommit();
    return att;
}

/// <summary>
/// Registers a hook
/// </summary>
/// <param name="orig">The original function found within MC, defined using CREATE_FUNC</param>
/// <param name="hook">The hook/replacement function, has to be static.</param>
/// <param name="onRegister">The function to call when the hook is registered</param>
/// <returns></returns>
inline LONG registerHook(PVOID* orig, PVOID hook, void (*onRegister)()) {
    LONG att = registerHook(orig, hook);
	onRegister();
    return att;
}

/// <summary>
/// Registers a vector of hooks
/// </summary>
/// <param name="hooks_vec">A vector of tuples (PVOID* orig, PVOID hook, void (*onRegister)())</param>
/// <param name="orig">The original function found within MC, defined using CREATE_FUNC</param>
/// <param name="hook">The hook/replacement function, has to be static.</param>
/// <param name="onRegister">The function to call when the hook is registered</param>
/// <returns></returns>
inline void registerHooks(std::vector<std::tuple<PVOID*, PVOID, void (*)()>> hooks_vec) {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    for (auto hook : hooks_vec) {
        LONG att = DetourAttach(std::get<0>(hook), std::get<1>(hook));
        hooks.push_back(std::tuple<PVOID*, PVOID>(std::get<0>(hook), std::get<1>(hook)));
        void (*onRegister)() = std::get<2>(hook);
        if (onRegister != nullptr)
            onRegister();
    }
    
    DetourTransactionCommit();    
}

/// <summary>
/// Registers a vector of hooks
/// </summary>
/// <param name="hooks_vec">A vector of tuples (PVOID* orig, PVOID hook, void (*onRegister)())</param>
/// <param name="orig">The original function found within MC, defined using CREATE_FUNC</param>
/// <param name="hook">The hook/replacement function, has to be static.</param>
/// <param name="onRegister">The function to call when the hook is registered</param>
/// <returns></returns>
inline void registerHooks(std::vector<std::tuple<PVOID*, PVOID>> hooks_vec) {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    for (auto hook : hooks_vec) {
        LONG att = DetourAttach(std::get<0>(hook), std::get<1>(hook));
        hooks.push_back(hook);
    }

    DetourTransactionCommit();
}

/// <summary>
/// Unregisters a vector of hooks
/// </summary>
/// <param name="hooks_vec">A vector of tuples (PVOID* orig, PVOID hook, void (*onUnregister)())</param>
/// <param name="orig">The original function found within MC, defined using CREATE_FUNC</param>
/// <param name="hook">The hook/replacement function, has to be static.</param>
/// <param name="onRegister">The function to call when the hook is unregistered</param>
/// <returns></returns>
inline void unregisterHooks(std::vector<std::tuple<PVOID*, PVOID, void (*)()>> hooks_vec) {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    for (auto hook : hooks_vec) {
        LONG att = DetourDetach(std::get<0>(hook), std::get<1>(hook));
        hooks.erase(std::remove(hooks.begin(), hooks.end(), std::tuple<PVOID*, PVOID>(std::get<0>(hook), std::get<1>(hook))), hooks.end());

        void (*onUnregister)() = std::get<2>(hook);
        if (onUnregister != nullptr)
            onUnregister();
    }

    DetourTransactionCommit();
}

/// <summary>
/// Unregisters a vector of hooks
/// </summary>
/// <param name="hooks_vec">A vector of tuples (PVOID* orig, PVOID hook)</param>
/// <param name="orig">The original function found within MC, defined using CREATE_FUNC</param>
/// <param name="hook">The hook/replacement function, has to be static.</param>
/// <returns></returns>
inline void unregisterHooks(std::vector<std::tuple<PVOID*, PVOID>> hooks_vec) {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    for (auto hook : hooks_vec) {
        LONG att = DetourDetach(std::get<0>(hook), std::get<1>(hook));
        hooks.erase(std::remove(hooks.begin(), hooks.end(), hook), hooks.end());
    }

    DetourTransactionCommit();
}