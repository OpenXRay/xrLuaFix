#include "pch.hpp"

#include "LibCompat.h"

// S.T.A.L.K.E.R. scripts has some specifics that must be taken into account:
// CS/COP scripts are written for LuaJIT 1.1.4
// SOC scripts are written for LuaJIT 1.0.3 with C-style comments support
namespace
{
constexpr auto s_compatibility_script =
R"(
    -- LuaJIT 1.x and LuaJIT 2.x prior to 2020 provides compatibility with Lua 5.0 function names:
    -- Lua 5.0: string.gfind,  math.mod
    -- Lua 5.1: string.gmatch, math.fmod
    if string then
        string.gfind = string.gfind or string.gmatch
    end
    if math then
        math.mod = math.mod or math.fmod
    end

    -- LuaJIT 1.x provides LuaCoco
    -- LuaJIT 2.x provides almost all of it's features (https://www.freelists.org/post/luajit/CoCo-in-202,1)
    -- So, let's pretend LuaCoco is supported if JIT is supported.
    if coroutine then
        coroutine.coco = coroutine.coco or jit
        coroutine.cstacksize = coroutine.cstacksize or function(newsize)
            return 0
        end
    end
)";
} // namespace

int open_stkcompat(lua_State* L)
{
    luaL_dostring(L, s_compatibility_script);

    return 0;
}
