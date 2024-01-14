CC=g++

all: src/main.cc src/handle_file.cc src/runner.cc src/lexer.cc src/parser.cc
	$(CC) -o app src/main.cc src/handle_file.cc src/runner.cc src/lexer.cc src/parser.cc

clean:
	rm -f app
