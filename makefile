CXX = g++
CXXFLAGS = -g
DEPS = hash_table.hpp
OBJ = main.o hash_table.o

%.o: %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

main.x: $(OBJ) $(DEPS)
	$(CXX) -o $@ $^ $(CXXFLAGS)
