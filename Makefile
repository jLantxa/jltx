BUILD := build
INCLUDE := include
SRC := src
TEST := test

CXXFLAGS += \
	-std=c++20 \
	-Os \
	-Wall -Werror

all: sockets utils run-tests

clean:
	rm -r ./$(BUILD)

format:
	clang-format --style=Google -i \
		$(SRC)/*/*.cpp \
		$(INCLUDE)/*/*.hpp \
		$(TEST)/*.cpp

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

TESTS_SOURCES += \
	$(SRC)/util/TextUtils.cpp \
	$(TEST)/TextUtilsTest.cpp
TESTS_TARGET := tests
tests:
	$(CXX) $(CXXFLAGS) \
		-lgtest_main -lgtest \
		-I $(INCLUDE) \
		$(TESTS_SOURCES) \
		-o $(BUILD)/jltx_$(TESTS_TARGET)

run-tests: tests
	./$(BUILD)/jltx_$(TESTS_TARGET)
