include config.mk

CONTAINERS_DIR = containers
CONTAINERS_H = $(wildcard $(CONTAINERS_DIR)/*.h)
CONTAINERS_SRC = $(wildcard $(CONTAINERS_DIR)/*.cpp)
CONTAINERS_OBJ = $(CONTAINERS_SRC:%.cpp=%.o)

$(CONTAINERS_DIR)/%.o: $(CONTAINERS_DIR)/%.cpp $(CONTAINERS_H)
	$(CXX) -c $< -o $@

all: containers

containers: $(CONTAINERS_OBJ)

clean:
	$(RM) $(CONTAINERS_OBJ)

.PHONY: containers all
