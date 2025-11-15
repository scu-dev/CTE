#pragma once
#include <string>
#include <vector>

namespace Util {
    typedef uint64_t u64;
    using std::string, std::vector;

    inline void toLowercase(string& str) noexcept {
        for (size_t i = 0; i < str.size(); i++) if (str[i] >= 'A' && str[i] <= 'Z') str[i] += 'a' - 'A';
    }

    inline void padStart(string& str, char ch, u64 total) noexcept {
        if (str.size() >= total) return;
        str = string(total - str.size(), ch) + str;
    }
}