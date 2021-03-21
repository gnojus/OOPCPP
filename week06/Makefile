include config.mk

CONTAINERS_DIR = containers
CONTAINERS_H = $(wildcard $(CONTAINERS_DIR)/*.h)
CONTAINERS_SRC = $(wildcard $(CONTAINERS_DIR)/*.cpp)
CONTAINERS_OBJ = $(CONTAINERS_SRC:%.cpp=%.o)

DOCS = doc/html

$(CONTAINERS_DIR)/%.o: $(CONTAINERS_DIR)/%.cpp $(CONTAINERS_H)
	$(CXX) $(CFLAGS) $(DEFINES) -c $< -o $@

all: containers build_tests main doc

containers: $(CONTAINERS_OBJ)

$(MAIN_TARGET): $(CONTAINERS_OBJ) main.cpp
	$(CXX) $(CFLAGS) $(DEFINES) $(CONTAINERS_OBJ) main.cpp -o $@

$(TESTS_TARGET): $(CONTAINERS_OBJ) doctest.h test.cpp
	$(CXX) $(CFLAGS) $(DEFINES) $(CONTAINERS_OBJ) test.cpp -o $@

build_tests: $(TESTS_TARGET)

run_tests: build_tests
	./$(TESTS_TARGET) --reporters=stderr,file --no-colors=true -o=$(LOGFILE)

rebuild: clean all

doc:
	$(DOXYGEN)

clean:
	$(RM) $(CONTAINERS_OBJ)
	$(RM) $(TESTS_TARGET)
	$(RM) $(MAIN_TARGET)
	$(RM) $(LOGFILE)
	$(RM) -r $(DOCS)

.PHONY: all run_tests clean doc build_tests rebuild containers
