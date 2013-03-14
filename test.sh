#!/bin/bash
declare -a DEPS=('setfattr' 'getfattr' 'mktemp' 'errno')

echo "= Checking dependencies (${DEPS[@]})"
for dep in ${DEPS[@]}; do
    if type $dep; then
        :
    else
        exit 1
    fi
done

TESTFILE=$(mktemp)
if [[ -z "$TESTFILE" ]]; then
    echo "= Error: Could not create temporary file. Abort."
    exit 1
else
    echo "= Using temporary file $TESTFILE"
fi

# test listing

echo "Test #1: Listing xattributes"
if ! setfattr "$TESTFILE" -n "Hello" -v "World!"; then
    echo "= Error: Could not prepare $TESTFILE. Abort."
    exit 1
else
    echo "= Attribute @ $TESTFILE: key=Hello value=World!"
    echo "= Try to list the attribute with Lua"
    echo "
local lx = require('lxattr')
local t, errno = lx.list('$TESTFILE')
if not t and not errno then
    print('LUA = lx.list('$TESTFILE'): error due to malformed argument.')
    os.exit(1)
elseif not t and errno then
    print('LUA = lx.list('$TESTFILE'): error propagated from libc call.')
    print('LUA = The errno message is:')
    os.exec('errno ' .. errno)
    os.exit(1)
elseif t and type(t) == 'table' then
    print('LUA = lx.list('$TESTFILE') returned a value of type table')
    print('LUA = lx.list('$TESTFILE'): will list attributes: index-name')
    for k,v in ipairs(t) do
        print(k,v)
    end
    os.exit(0)
end
os.exit(1)" > "$TESTFILE.1"
    if lua "$TESTFILE.1"; then
        echo "= Test #1: PASSED"
    else
        echo "= Test #1: FAILED"
        echo "= It makes no sense to continue. Abort."
        exit 1
    fi
fi
