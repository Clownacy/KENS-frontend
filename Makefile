CXXFLAGS := -O3 -std=c++11

bin/kensfrontend.exe: kensfrontend.c KENS-source/GetFileSize.cpp KENS-source/Kosinski/K-Compressor.cpp KENS-source/Kosinski/K-Decompressor.cpp KENS-source/Enigma/E-Compressor.cpp KENS-source/Enigma/E-Decompressor.cpp KENS-source/Nemesis/N-Compressor.cpp KENS-source/Nemesis/N-Decompressor.cpp KENS-source/Saxman/S-Compressor.cpp KENS-source/Saxman/S-Decompressor.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^
