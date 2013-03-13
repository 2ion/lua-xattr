#lua-xattr

A library for getting and setting extended file attributes on Linux. It
makes the setxattr/getxattr/listxattr function family from libattr
available in Lua. 

The project's home is at https://github.com/2ion/lxattr.

#Building

##Requirements
* Lua development files (either 5.1 or 5.2)
* libattr development files (we expect to have <attr/xattr.h> in our
  include path)
* pkg-config to query --cflags and --libs

##Commands
    # build for Lua 5.2
    make

    # build for Lua 5.1
    LUA=5.1 make

#Documentation

    local lx = require 'lxattr'

    --- set an attribute on a file, dereferencing symlinks
    -- @param path (non-empty string) relative or absolute file path
    -- @param name (non-empty string) name of the attribute
    -- @param value (string) value of the attribute
    -- @return TRUE if successful; NIL if a parameter does not meet our
    -- expecations; NIL and an INTEGER if a C function returned an error.
    -- The integer is glibc's errno error code.
    local s, errno = lx.set(path, name, value)

    --- set an attribute on a file, NOT following symlinks
    -- @see lx.set()
    local s, errno = lx.lset(path, name, value)

