CXXFLAGS := -O2 -s -Wall -Wextra -pedantic -std=c++98

all: bin/kensfrontend

bin/kensfrontend: main.c KENS-source/GetFileSize.cpp KENS-source/Kosinski/K-Compressor.cpp KENS-source/Kosinski/K-Decompressor.cpp KENS-source/Enigma/E-Compressor.cpp KENS-source/Enigma/E-Decompressor.cpp KENS-source/Nemesis/N-Compressor.cpp KENS-source/Nemesis/N-Decompressor.cpp KENS-source/Saxman/S-Compressor.cpp KENS-source/Saxman/S-Decompressor.cpp
	@mkdir -p bin
	@$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)
