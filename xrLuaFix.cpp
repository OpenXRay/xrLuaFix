#include "pch.hpp"

#include "xrLuaFix.h"

#include "LibStr.h"
#include "LibMath.h"
#include "LibTab.h"
#include "Logger.h"
#include "LibCompat.h"

#include "lfs.h"

#include <string.h>

extern "C" int luaopen_lua_pack(lua_State* L);
extern "C" int luaopen_marshal(lua_State* L);

// Checks global _VERSION and jit.version_num for signs of LuaJIT 1.x.x
static bool is_luajit_one(lua_State* L) noexcept
{
    lua_getglobal(L, "_VERSION");
    const char* version = lua_tostring(L, -1);
    bool is_luajit_one = version && strstr(version, "LuaJIT 1.");
    lua_pop(L, 1);
    if (is_luajit_one)
        return true;

    lua_getglobal(L, "jit");
    if (!lua_istable(L, -1))
        lua_pop(L, 1);
    else
    {
        lua_getfield(L, -1, "version_num");
        const int version_num = static_cast<int>(lua_tointeger(L, -1));
        is_luajit_one = version_num >= 10000 && version_num < 20000;
        lua_pop(L, 2);
    }

    return is_luajit_one;
}

// Original xrLuaFix was extending lua libs only when global RvP function is called
// Starting from Call of Chernobyl, engine always does that.
int luaopen_xrluafix(lua_State* L)
{
    // Original xrLuaFix for Shadow of Chernobyl
    luaL_dostring(L, "RvP = function() end");

    // Anomaly compatibility
    luaL_register(L, "lua_extensions", nullptr);

    open_string(L);
    open_table(L);
    open_log(L);

    // LuaJIT 2.x generally has good RNG,
    // so override math library only for LuaJIT 1.x.x.
    // Don't touch if it's not LuaJIT either.
    if (is_luajit_one(L))
        open_math(L);
    else
        open_stkcompat(L);

    luaopen_lua_pack(L);
    luaopen_marshal(L);

    // luafilesystem adheres to modules-create-no-globals
    // (https://github.com/lunarmodules/luafilesystem/commit/8e7217e74fbe5da0a9c1fee03d191b5a0266cedd),
    // thus requiring you to write "local lfs = require("lfs")" in scripts.
    luaopen_lfs(L);
    // But we want to create globals exactly, so that you can use it right away.
    lua_setglobal(L, "lfs");

    return 0;
}
