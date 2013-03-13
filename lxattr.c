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

#include <stdlib.h>
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

static int my_listxattr(lua_State *L)
{
    size_t len = 0;
    size_t i = 0;
    ssize_t rcount, listlen;
    char **list = NULL;
    const char *path = lua_tolstring(L, 1, &len);
    if(len == 0) {
        lua_pushnil(L);
        return 1;
    }
    if(access(path, F_OK | R_OK) != 0) {
        lua_pushnil(L);
        lua_pushinteger(L, errno);
        return 2;
    }
    listlen = listxattr(path, NULL, (size_t) 0);
    if(listlen == -1) {
        lua_pushnil(L);
        lua_pushinteger(L, errno);
        return 2;
    } else if(listlen == 0) {
        // file without attributes
        lua_newtable(L);
        return 1;
    }
    // len > 0 := number of attributes
    // FIXME: maximum length of attribute names?
    // Let's assume sth for now ...
    list = (char**) calloc(sizeof(char)*1024*70000, (size_t) listlen);
    if(list == NULL) {
        lua_pushnil(L);
        lua_pushinteger(L, errno);
        return 2;
    }
    rcount = listxattr(path, (char*) list, (size_t) listlen);
    lua_newtable(L);
    for(i = 0; i < (size_t) rcount; i++) {
        lua_pushnumber(L, i+1);
        lua_pushstring(L, list[i]);
        lua_rawset(L, -3);
    }
    free(list);
    return 1; // table is on top of the stack
}

static const struct luaL_Reg lxattr_list[] = {
    { "set", my_setxattr },
    { "lset", my_lsetxattr },
    { "list", my_listxattr },
    { NULL, NULL }
};

int luaopen_lxattr(lua_State *L)
{
    luaL_newlib(L, lxattr_list);
    return 1;
}
