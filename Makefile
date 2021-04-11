CXX=g++

CXX_FLAGS=-Iinclude -Ibuild -g
CXX_FLAGS_TEST=-Iinclude -Ibuild -lgtest -lgtest_main -g -lgmock -lpthread

SOURCES=$(wildcard source/*.cpp)
OBJECTS=$(patsubst source/%.cpp, build/%.o, $(SOURCES))

GENERATED=build/parser.h build/parser.cpp
GENERATED_OBJECTS=build/parser.o

TEST_SOURCES=$(wildcard test/*.cpp)
TEST_OBJECTS=$(patsubst test/%.cpp, build/%.o, $(TEST_SOURCES))

$(GENERATED_OBJECTS): build/%.o: build/%.cpp | build
	$(CXX) $(CXX_FLAGS) -c $< -o $@

$(OBJECTS): build/%.o: source/%.cpp $(GENERATED) | build/
	$(CXX) $(CXX_FLAGS) -c $< -o $@

$(TEST_OBJECTS): build/%.o: test/%.cpp $(GENERATED) | build/
	$(CXX) $(CXX_FLAGS_TEST) -c $< -o $@

build/parser.h: tools/lc2_parser/*.py | build/
	python3.9 -m pip install -e tools
	gen-parser-h > $@

build/parser.cpp:tools/lc2_parser/*.py build/parser.h | build/
	python3.9 -m pip install -e tools
	gen-parser-cpp > $@

build/:
	mkdir -p $@

build/test: $(TEST_OBJECTS) $(GENERATED_OBJECTS) $(OBJECTS)
	$(CXX) -lgtest_main -o $@ $^ $(CXX_FLAGS_TEST)

build/lc2: $(OBJECTS) $(GENERATED_OBJECTS)
	$(CXX) -o $@ $(CXX_FLAGS) $^ 

.PHONY: test check clean

test: build/test
	$^ --gtest_output=xml

check: build/parser.h
	cppcheck source/ --std=c++11 -Iinclude -Ibuild --enable=all --error-exitcode=1 --suppress=missingIncludeSystem

clean:
	rm -rf build