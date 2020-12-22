BUILD := build
INCLUDE := include
SRC := src

CXXFLAGS += \
	-std=c++20 \
	-Os \
	-Wall -Werror

all: sockets

clean:
	rm -r $(BUILD)


SOCKETS_SOURCES += \
	$(SRC)/net/Socket.cpp
SOCKETS_TARGET := jltx_sockets
sockets:
	$(CXX) $(CXXFLAGS) \
		-I $(INCLUDE) -fpic $(SOCKETS_SOURCES) -shared \
		-o $(BUILD)/$(SOCKETS_TARGET).so

