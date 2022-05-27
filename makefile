CXX = g++
CXXFLAGS = -g -lpthread

banker_algorithm.x: src/main.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS)

clean:
	rm -f *.o *.x
