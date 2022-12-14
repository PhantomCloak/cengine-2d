#include "lua_manager.h"
#include <filesystem>

sol::state LuaManager::luaState;

sol::table LuaManager::LoadLuaTable(std::string key) {
    return luaState[key];
}

void LuaManager::InitSandbox() {
    luaState.open_libraries(sol::lib::base);
    luaState.open_libraries(sol::lib::table);
}

void LuaManager::LoadLuaFilesInDirectory(std::string path) {
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        luaState.script_file(entry.path());
    }
}

//template <typename... Args>
//void LuaManager::RegisterCppToLuaFunc(const std::string key, Args&&... args) {
    //luaState.set_function(key.c_str(), std::forward<Args>(args)...);
//}
