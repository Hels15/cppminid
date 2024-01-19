CC=g++

all: src/main.cc src/handle_file.cc src/runner.cc src/lexer.cc src/parser.cc
	$(CC) -fsanitize=undefined -o app src/main.cc src/handle_file.cc src/runner.cc src/lexer.cc src/parser.cc -g

clean:
	rm -f app
