CXXFLAGS := -O3 -std=c++11

bin/kensfrontend.exe: kensfrontend.c
	$(CXX) $(CXXFLAGS) -o $@ $^
