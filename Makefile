CXXFLAGS=-W -Wall -pedantic -std=c++17 -MD -MP -D_GLIBCXX_DEBUG

main: main.o gui.o
	$(CXX) -o $@ $^

-include *.d
