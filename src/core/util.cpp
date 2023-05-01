#include "util.h"

constexpr std::size_t Util::fnv_hash(const char* str, size_t hash) {
    return (*str == '\0') ? hash : fnv_hash(str + 1, (hash ^ static_cast<std::size_t>(*str)) * 16777619u);
};

std::size_t Util::fnv_hash_dynamic(const char* str, std::size_t hash) {
    return (*str == '\0') ? hash : fnv_hash(str + 1, (hash ^ static_cast<std::size_t>(*str)) * 16777619u);
}
