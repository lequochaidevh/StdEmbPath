
PRO_DIR 	:=.
PROJ_NAME	:= GPIO
OUTPUT_PATH 	:= $(PRO_DIR)/output

main.o: source/main.c
	gcc -c source/main.c -o output/main.o

build: output/main.o
	gcc output/main.o -o output/prgexcute
run:
	./output/prgexcute
.PHONY: clean
clean:
	rm output/*
print-%:
	@echo $@
	echo $($(subst print-,,$@))
