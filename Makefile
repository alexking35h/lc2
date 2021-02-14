CXX_FLAGS=-Iinclude -g
CXX_FLAGS_TEST=-Iinclude -lgtest -lgtest_main -g -lgmock -lpthread

SOURCES=$(wildcard source/*.cpp)
TEST_SOURCES=$(wildcard test/*.cpp)
OBJECTS=$(patsubst source/%.cpp, build/%.o, $(SOURCES))
TEST_OBJECTS=$(patsubst test/%.cpp, build/%.o, $(TEST_SOURCES))

$(OBJECTS): build/%.o: source/%.cpp | build/
	$(CXX) $(CXX_FLAGS) -c $^ -o $@

$(TEST_OBJECTS): build/%.o: test/%.cpp | build/
	$(CXX) $(CXX_FLAGS_TEST) -c $^ -o $@

build/:
	mkdir -p $@

build/test: $(TEST_OBJECTS) $(OBJECTS)
	$(CXX) $(CXX_FLAGS_TEST) -o $@ $^

.PHONY: test clean

test: build/test
	$^

clean:
	rm -rf build

all:
	echo $(SOURCES)
	echo $(OBJECTS)
