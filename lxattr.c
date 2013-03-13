/*
 * lxattr
 * Lua library for setting and getting xattributes on Linux.
 *
 * Copyright 2013 Jens Oliver John <jens.o.john // gmail.com>
 * Licensed under the GNU General Public License v3 or later.
 *
 * Project home: https://github.com/2ion/lxattr
 * Documentation: See /README.md in the source tree
 * */

#include <sys/types.h>
#include <attr/xattr.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <lua.h>
#include <lauxlib.h>

static int my_setxattr(lua_State *L)
{
    size_t plen, nlen, vlen;
    const char *path = lua_tolstring(L, 1, &plen);
    const char *aname = lua_tolstring(L, 2, &nlen);
    const char *avalue = lua_tolstring(L, 3, &vlen);
    if(plen == 0 || nlen == 0) {
        lua_pushnil(L);
        return 1;
    }
    if(access(path, F_OK | W_OK) != 0) {
        lua_pushnil(L);
        lua_pushinteger(L, errno);
        return 2;
    }
    if(setxattr(path, aname, avalue, vlen, 0) == 0) {
        lua_pushboolean(L, 1);
        return 1;
    } else {
        lua_pushnil(L);
        lua_pushinteger(L, errno);
        return 2;
    }
    return 0;
}

static int my_lsetxattr(lua_State *L)
{
    size_t plen, nlen, vlen;
    const char *path = lua_tolstring(L, 1, &plen);
    const char *aname = lua_tolstring(L, 2, &nlen);
    const char *avalue = lua_tolstring(L, 3, &vlen);
    if(plen == 0 || nlen == 0) {
        lua_pushnil(L);
        return 1;
    }
    if(faccessat(AT_FDCWD, path, F_OK | W_OK, AT_SYMLINK_NOFOLLOW) != 0) {
        lua_pushnil(L);
        lua_pushinteger(L, errno);
        return 2;
    }
    if(setxattr(path, aname, avalue, vlen, 0) == 0) {
        lua_pushboolean(L, 1);
        return 1;
    } else {
        lua_pushnil(L);
        lua_pushinteger(L, errno);
        return 2;
    }
    return 0;
}


static const struct luaL_Reg lxattr_list[] = {
    { "set", my_setxattr },
    { "lset", my_lsetxattr },
    { NULL, NULL }
};

int luaopen_lxattr(lua_State *L)
{
    luaL_newlib(L, lxattr_list);
    return 1;
}
