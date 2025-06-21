.PHONY: clean

main.o: source/main.c
	gcc -c source/main.c -o output/main.o

build: output/main.o
	gcc output/main.o -o output/prgexcute
run:
	./output/prgexcute
clean:
	rm output/*
