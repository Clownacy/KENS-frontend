CFLAGS := -O2 -s -static -Wall -Wextra -pedantic -std=c99

all: bin/kensfrontend

bin/kensfrontend: main.c
	@mkdir -p bin
	@mkdir -p bin/libs
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)
	@cp -u KENS/*.dll bin/libs
