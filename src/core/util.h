#pragma once
#include <cstddef>

class Util {
    public:
    static constexpr std::size_t fnv_hash(const char* str, std::size_t hash = 2166136261u);
    static std::size_t fnv_hash_dynamic(const char* str, std::size_t hash = 2166136261u);
};
