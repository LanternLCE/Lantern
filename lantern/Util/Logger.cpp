#include "pch.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <locale>
#include <codecvt>
#include "Logger.h"

std::string ToHex(size_t v) {
    std::ostringstream oss;
    oss << std::hex << std::uppercase << v;
    return oss.str();
}