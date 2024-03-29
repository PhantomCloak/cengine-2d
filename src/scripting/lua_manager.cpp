#include "lua_manager.h"
#include "../log/log.h"
#include <filesystem>

sol::state LuaManager::luaState;

sol::table LuaManager::LoadLuaTable(std::string key) {
    return luaState[key];
}

void LuaManager::InitSandbox() {
    luaState.open_libraries(sol::lib::base);
    luaState.open_libraries(sol::lib::package);
    luaState.open_libraries(sol::lib::string);
    luaState.open_libraries(sol::lib::table);
    luaState.open_libraries(sol::lib::math);
    luaState.open_libraries(sol::lib::coroutine);

    const std::string package_path = luaState["package"]["path"];

    char* luaLibPathEnv = std::getenv("COMMANCHE_LUA_LIBS");
    std::string luaLibPath;

    if (luaLibPathEnv != nullptr) {
        luaLibPath = std::string(luaLibPathEnv);
    } else {
        luaLibPath = "./assets/scripts/lib";
    }

    Log::Inf("lua path pointing to %s", luaLibPath.c_str());

    luaState["package"]["path"] = package_path + ";" + luaLibPath;
}

void LuaManager::LoadLuaFilesInDirectory(std::string path) {
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        std::string entryPath = entry.path().string();
        luaState.script_file(entryPath);
        continue;
    }
}

void LuaManager::LoadLuaFile(const std::string path) {
    luaState.script_file(path);
}
