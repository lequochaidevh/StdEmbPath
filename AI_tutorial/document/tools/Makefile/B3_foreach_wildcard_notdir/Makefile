
#SRC_DIRS	= /home/devh/Documents/LearnMakefile/B3_foreach_wildcard_notdir/source
#SRC_DIRS	+= /home/devh/Documents/LearnMakefile/B3_foreach_wildcard_notdir/source1

SRC_DIRS	= /home/devh/zip/3_Embedded_mcu/devh_2022/Documents/LearnMakefile/B3_foreach_wildcard_notdir/source

SRC_FILES	:= $(foreach SRC_DIRS,$(SRC_DIRS),$(wildcard $(SRC_DIRS)/*))
#SRC_DIRS	:= $(wildcard $(SRC_DIRS)/*)

OBJ_FILES	:= $(notdir $(SRC_FILES))
OBJ_FILES	:= $(subst .c,.o,$(OBJ_FILES))

build: $(OBJ_FILES)
	gcc output/main.o -o output/excute
main.o: source/main.c
	gcc -c source/main.c -o output/main.o


run:
	./output/excute
.PHONY: clean
clean:
	rm output/*
print-%:
	@echo $($(subst print-,,$@))
