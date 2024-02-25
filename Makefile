CXX = gcc
CXXFLAGS = -O2
LDFLAGS = -g -I../include/ -lvulkan -ldl -lpthread -lXxf86vm -lXi -lwayland-client -lrt -lxkbcommon -Wextra -Wall -DWLR_USE_UNSTABLE -lstdc++

SRC = mycli.cpp engine.cpp ../include/xdg-shell-protocol.c ../include/wp-single-pixel-buffer-v1-client-protocol.c ../include/viewporter-client-protocol.c
TARGET = myclipp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)