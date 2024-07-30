CC=clang

OTHER_FLAGS=-g -DDEBUG=1

CFLAGS=-std=c11 ${OTHER_FLAGS} -Iinclude

DEPS=\
	source/scs/scs.o \
	source/scs/arrayhelper.o \

LINKS=

all: $(DEPS)

.PHONY: clean format

format:
	@find ./include -name '*.h' | xargs clang-format -style=file -i
	@find ./source -name '*.c'  | xargs clang-format -style=file -i
	@find ./examples -name '*.c'  | xargs clang-format -style=file -i
	@find ./test -name '*.cpp'  | xargs clang-format -style=file -i
	

clean:
	@find . -name '*.o' -delete
	@find . -name '*.out' -delete
	@rm -f *.out