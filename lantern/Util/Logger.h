#pragma once
#include <iostream>
#include <sstream>
#include <iomanip>
//#include "Lantern.h"

#ifdef LANTERN_EXPORTS
    #define LOGGER_API __declspec(dllexport)
#else  
    #define LOGGER_API __declspec(dllimport)
#endif

// TODO: Logging to file to be readded when events exist... I need to sleep ASAP.

enum ANSIColor : uint8_t {
    RESET = 0,
    BOLD = 1,
    ITALIC = 3,
    UNDERLINE = 4,
    STRIKETHROUGH = 9,
    BLACK = 30,
    RED = 31,
    GREEN = 32,
    YELLOW = 33,
    BLUE = 34,
    MAGENTA = 35,
    CYAN = 36,
    WHITE = 37,
    BLACK_BG = 40,
    RED_BG = 41,
    GREEN_BG = 42,
    YELLOW_BG = 43,
    BLUE_BG = 44,
    MAGENTA_BG = 45,
    CYAN_BG = 46,
    WHITE_BG = 47
};

// TODO: Log the mod's name
#define LOG(...) log(__func__, __VA_ARGS__)
#define LOGW(...) logw(__func__, __VA_ARGS__)
#define LOG_C(color, ...) log_c(__func__, color, __VA_ARGS__)
#define LOGW_C(color, ...) logw_c(__func__, color, __VA_ARGS__)

static std::string ToHex(size_t v);

template<typename... Args>
static void log(const char* funcName, const Args&... args) {
    std::cout << "\033[96m";
    (std::cout << "[" << funcName << "] ");
    std::cout << "\033[0m";

    std::cout << "\033[36m";
    (std::cout << ... << args);
    std::cout << "\033[0m" << std::endl;
    // MIGHT BE INEFFICIENT
    //if (Lantern::logFile != nullptr) {
    //    std::wstring wfunc(funcName, funcName + strlen(funcName));
    //    std::wostringstream str;
    //    (str << ... << args);
    //    Lantern::logFile->WriteLine(L"[" + wfunc + L"] " + str.str());
    //}
}

template<typename... Args>
static void logw(const char* funcName, const Args&... args) {
    std::wcout << L"\033[96m";
    (std::cout << "[" << funcName << "] ");
    std::wcout << L"\033[0m";

    std::wcout << L"\033[36m";
    (std::wcout << ... << args);
    std::wcout << L"\033[0m" << std::endl;
    //if (Lantern::logFile != nullptr) {
    //    std::wstring wfunc(funcName, funcName + strlen(funcName));
    //    std::wostringstream str;
    //    (str << ... << args);
    //    Lantern::logFile->WriteLine(L"[" + wfunc + L"] " + str.str());
    //}
}

template<typename... Args>
static void log_c(const char* funcName, ANSIColor color, const Args&... args) {
    std::cout << "\033[96m";
    (std::cout << "[" << funcName << "] ");
    std::cout << "\033[0m";

    std::cout << "\033[" << color << "m";
    (std::cout << ... << args);
    std::cout << "\033[0m" << std::endl;
    //if (Lantern::logFile != nullptr) {
    //    std::wstring wfunc(funcName, funcName + strlen(funcName));
    //    std::wostringstream str;
    //    (str << ... << args);
    //    Lantern::logFile->WriteLine(L"[" + wfunc + L"] " + str.str());
    //}
}

template<typename... Args>
static void logw_c(const char* funcName, ANSIColor color, const Args&... args) {
    std::wcout << L"\033[96m";
    (std::cout << "[" << funcName << "] ");
    std::wcout << L"\033[0m";

    std::wcout << "\033[" << color << "m";
    (std::wcout << ... << args);
    std::wcout << "\033[0m" << std::endl;
    //if (Lantern::logFile != nullptr) {
    //    std::wstring wfunc(funcName, funcName + strlen(funcName));
    //    std::wostringstream str;
    //    (str << ... << args);
    //    Lantern::logFile->WriteLine(L"[" + wfunc + L"] " + str.str());
    //}
}