#lua-xattr

*PARTIAL IMPLEMENTATION*

A library for getting and setting extended file attributes on Linux. It
makes the setxattr/getxattr/listxattr function family from libattr
available in Lua. 

The project's home is at https://github.com/2ion/lua-xattr

#Building

##Requirements
* Lua 5.2 development files 
* libattr development files (we expect to have <attr/xattr.h> in our
  include path)
* pkg-config to query --cflags and --libs

##Commands
    make

#Documentation

```lua
    local lx = require 'lxattr'

    --- set an attribute on a file, dereferencing symlinks
    -- @param path (non-empty string) relative or absolute file path
    -- @param name (non-empty string) name of the attribute
    -- @param value (string) value of the attribute
    -- @return TRUE if successful; NIL if a parameter does not meet our
    -- expecations; NIL and an INTEGER if a C function returned an error.
    -- The integer is glibc's errno error code.
    local bool, errno = lx.set(path, name, value)

    --- set an attribute on a file, NOT following symlinks
    local bool, errno = lx.lset(path, name, value)

    --- list a file's attributes, following symlinks
    -- @return attribute names (list of strings)
    local table, errno = lx.list(path)

    --- list a file's attributes, NOT following symlinks
    -- @return like list()
    local table, errno = lx.llist(path)

    -- get()/lget() are not yet implemented!
```
