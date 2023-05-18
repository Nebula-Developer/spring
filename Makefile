.PHONY: run build

run:
	@make build
	@./bin/spring

build:
	@mkdir -p bin
	@g++ -o bin/spring src/main.cpp -ltermcap
