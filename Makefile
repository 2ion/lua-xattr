all:
	gcc -O2 $(shell pkg-config --libs --cflags lua5.2) -o lxattr.so lxattr.c -shared -fPIC

