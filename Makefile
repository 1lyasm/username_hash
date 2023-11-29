all:
	clang -g -fsanitize=address -Wall \
		-Wpedantic -ansi src/main.c -o main
normal:
	./main NORMAL
debug:
	./main DEBUG
