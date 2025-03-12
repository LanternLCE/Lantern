#pragma once

#ifdef LANTERN_EXPORTS
#define LANTERN_CLASS __declspec(dllexport)
#else  
#define LANTERN_CLASS __declspec(dllimport)
#endif

//#include "Util/LogFile.h";

class Lantern {
public:
    #define LANTERN_VERSION L"1.2.0.0"

    static class LANTERN_CLASS Version {
    public:
        uint16_t major;
        uint16_t minor;
        uint16_t build;
        uint16_t patch;

        Version(std::wstring version);
        Version(uint16_t major, uint16_t minor, uint16_t build, uint16_t patch);
        Version(uint64_t packed);
        uint64_t GetPackedVersion();
        std::wstring GetVersionString();
    };

	static LANTERN_CLASS Version version;
};