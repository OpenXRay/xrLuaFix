add_library(xrLuaFix STATIC)

target_sources(xrLuaFix PRIVATE
    LibMath.cpp
    LibMath.h
    LibStr.cpp
    LibStr.h
    LibTab.cpp
    LibTab.h
    Logger.cpp
    Logger.h
    pch.cpp
    pch.hpp
    xrLuaFix.cpp
    xrLuaFix.h
    lfs/src/lfs.c
    lfs/src/lfs.h
    lua-marshal/lmarshal.c
    lua-pack/lua_pack.c
)

target_include_directories(xrLuaFix
    PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"

    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/lfs/src"
    "${CMAKE_SOURCE_DIR}/src"
)

target_link_libraries(xrLuaFix
    PRIVATE
    xrCore
)

if (XRAY_USE_LUAJIT)
    target_link_libraries(xrLuaFix PUBLIC xrLuaJIT)
else()
    target_link_libraries(xrLuaFix PUBLIC Lua51)
endif()

target_precompile_headers(xrLuaFix
    PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:pch.hpp>
)

set_target_properties(xrLuaFix PROPERTIES
    PREFIX ""
    POSITION_INDEPENDENT_CODE ON
)
