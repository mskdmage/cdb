default:
	@gcc -I ./inc -o ./bin/main src/*.c -Wall

execute:
	@echo "\nBINARY EXECUTION\n" &&\
	./bin/main

run: default test execute

test:
	@valgrind --leak-check=full -s ./bin/main
