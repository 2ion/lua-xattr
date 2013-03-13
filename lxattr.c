#include <sys/types.h>
#include <attr/xattr.h>
#include <unistd.h>
#include <errno.h>
#include <lua.h>
#include <lauxlib.h>

// path::str, name::str, value::str, 0
static int my_setxattr(lua_State *L)
{
    int e = 0;
    size_t plen, nlen, vlen;
    const char *path = lua_tolstring(L, 1, &plen);
    const char *aname = lua_tolstring(L, 2, &nlen);
    const char *avalue = lua_tolstring(L, 3, &vlen);
    if(plen == 0 || nlen == 0 || avalue == 0) {
        lua_pushnil(L);
        lua_pushinteger(L, e);
        return 2;
    }
    if((e = access(path, F_OK | W_OK)) != 0) {
        lua_pushnil(L);
        lua_pushinteger(L, e);
        lua_pushinteger(L, errno);
        return 3;
    }
    e = setxattr(path, aname, avalue, vlen, 0);
    if(e == 0) {
        lua_pushboolean(L, 1);
        lua_pushinteger(L, e);
        return 2;
    } else {
        lua_pushnil(L);
        lua_pushinteger(L, e);
        lua_pushinteger(L, errno);
        return 3;
    }
    return 0;
}

static int my_lsetxattr(lua_State *L)
{
    return 0;

}


static const struct luaL_Reg lxattr_list[] = {
    { "setxattr", my_setxattr },
    { NULL, NULL }
};

int luaopen_lxattr(lua_State *L)
{
    luaL_newlib(L, lxattr_list);
    return 1;
}
