CXXFLAGS := -O3 -s -std=c++11

bin/kensfrontend.exe: main.c
	$(CXX) $(CXXFLAGS) -o $@ $^
