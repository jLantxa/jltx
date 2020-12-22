BUILD := build
INCLUDE := include
SRC := src

CXXFLAGS += \
	-std=c++20 \
	-Os \
	-Wall -Werror

clean:
	rm -r $(BUILD)
