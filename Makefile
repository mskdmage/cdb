default:
	@gcc -I ./include -o ./bin/main src/*.c -Wall

execute:
	@./bin/main

run: default execute
