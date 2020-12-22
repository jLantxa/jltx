BUILD := build
INCLUDE := include
SRC := src

CXXFLAGS += \
	-std=c++20 \
	-Os \
	-Wall -Werror

all: sockets utils

clean:
	rm -r $(BUILD)


SOCKETS_SOURCES += \
	$(SRC)/net/Socket.cpp
SOCKETS_TARGET := jltx
sockets:
	$(CXX) $(CXXFLAGS) \
		-I $(INCLUDE) -fpic $(SOCKETS_SOURCES) -shared \
		-o $(BUILD)/$(SOCKETS_TARGET)


UTILS_SOURCES += \
	$(SRC)/util/TextUtils.cpp
UTILS_TARGET := utils
utils:
	$(CXX) $(CXXFLAGS) \
		-I $(INCLUDE) -fpic $(UTILS_SOURCES) -shared \
		-o $(BUILD)/jltx_$(UTILS_TARGET).so
