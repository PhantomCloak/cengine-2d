#pragma once
#include <sol/sol.hpp>

class LuaManager {
    private:
    public:
    static sol::state luaState;
    static void InitSandbox();
    static void LoadLuaFilesInDirectory(const std::string path);
    static void LoadLuaFile(const std::string path);
    static sol::table LoadLuaTable(std::string key);
    template <typename... Args>
    static void RegisterCppToLuaFunc(const std::string key, Args&&... args){
      luaState.set_function(key.c_str(), std::forward<Args>(args)...);
    }
};
