BUILD := build
INCLUDE := include
SRC := src
TEST := test
EXAMPLES := examples
DOC := doc

CXXFLAGS += \
	-std=c++17 \
	-Wall -Werror \
	-O2 \
	-Wconversion


all: mkdir sockets utils nco_tonegen doc tests


mkdir:
	@mkdir -p $(BUILD)


clean:
	rm -r ./$(BUILD)
	rm -r ./$(DOC)


format:
	clang-format --style=Google -i \
		$(SRC)/*/*.cpp \
		$(INCLUDE)/*/*.hpp \
		$(TEST)/*.cpp \
		$(EXAMPLES)/*.cpp


doc:
	doxygen


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


NCO_TONE_GEN_SOURCES += \
	$(EXAMPLES)/nco_tonegen.cpp \
	$(SRC)/audio/AlsaAudioSink.cpp
NCO_TONE_GEN_TARGET := nco_tonegen
nco_tonegen:
	$(CXX) $(CXXFLAGS) \
		-I $(INCLUDE) \
		$(NCO_TONE_GEN_SOURCES) \
		-lasound \
		-o $(BUILD)/jltx_$(NCO_TONE_GEN_TARGET)


TESTS_SOURCES += \
	$(SRC)/util/TextUtils.cpp \
	$(TEST)/TextUtilsTest.cpp \
	$(TEST)/RingArrayTest.cpp \
	$(TEST)/MathTest.cpp
TESTS_TARGET := tests
tests:
	$(CXX) $(CXXFLAGS) \
		-lgtest_main -lgtest \
		-I $(INCLUDE) \
		$(TESTS_SOURCES) \
		-o $(BUILD)/jltx_$(TESTS_TARGET)
	./$(BUILD)/jltx_$(TESTS_TARGET)
