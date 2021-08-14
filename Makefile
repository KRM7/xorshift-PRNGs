CXX := g++
CC := g++

WARN_FLAGS := -Wall -Wextra -pedantic
FLAGS := -std=c++17 -DNDEBUG -O2 -flto $(WARN_FLAGS)

SRC := benchmark.cpp
BIN := benchmark

benchmark: $(SRC)
	$(CXX) $(FLAGS) $(SRC) -o $(BIN)

clean:
	$(RM) *.exe