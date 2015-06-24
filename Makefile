CXXFLAGS := -O3

bin/kensfrontend.exe: kensfrontend.c
	$(CXX) $(CXXFLAGS) -o $@ $^
