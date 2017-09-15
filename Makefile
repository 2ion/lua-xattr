LUA = lua5.2
CFLAGS = -shared -fPIC -Wall -O2 $(shell pkg-config --cflags $(LUA))
LDFLAGS = $(shell pkg-config --libs $(LUA))
TARGET = lxattr

$(TARGET).so: $(TARGET).c
	gcc $(CFLAGS) $(LDFLAGS) $(TARGET).c -o $(TARGET).so 

clean:
	-rm $(TARGET).so
