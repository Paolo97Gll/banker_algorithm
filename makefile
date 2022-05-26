CXX = g++
CXXFLAGS = -g -lpthread

main.x: main.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS)

clean:
	rm -f *.o *.x
