CXXFLAGS := -O3 -std=c++11

bin/kensfrontend.exe: main.c
	$(CXX) $(CXXFLAGS) -o $@ $^
